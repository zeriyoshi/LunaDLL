#include <climits>
#include <tuple>
#include <algorithm>
#include "../Globals.h"
#include "Rendering.h"
#include "RenderUtils.h"
#include "../Defines.h"
#include "../SMBXInternal/Level.h"
#include "../Misc/MiscFuncs.h"
#include "BMPBox.h"
#include "RenderOps/RenderOp.h"
#include "RenderOps/RenderRectOp.h"
#include "RenderOps/RenderGLOp.h"
#include "RenderOps/RenderStringOp.h"
#include "../SMBXInternal/PlayerMOB.h"
#include "../GlobalFuncs.h"
#include "GL/GLEngine.h"
#include "BitBltEmulation.h"

using namespace std;

// CTOR
Renderer::Renderer() :
    m_InFrameRender(false),
    m_curCamIdx(1),
    m_renderOpsSortedCount(0),
    m_renderOpsProcessedCount(0)
{
}

// DTOR
Renderer::~Renderer() {
}

// LOAD BITMAP RESOURCE - Load an image resource with given resource code. If resource code exists, replaces old image
bool Renderer::LoadBitmapResource(std::wstring filename, int resource_code, int transparency_color) {
    if (Renderer::LoadBitmapResource(filename, resource_code)) {

        auto it = m_legacyResourceCodeImages.find(resource_code);
        if (it != m_legacyResourceCodeImages.end()) {
            it->second->MakeColorTransparent(transparency_color);
        }
        return true;
    }

    return false;
}

bool Renderer::LoadBitmapResource(std::wstring filename, int resource_code) {
    // If slot in use, delete old image
    DeleteImage(resource_code);

    // Concoct full filepath
    wstring full_path = L"";

    if (!isAbsolutePath(filename)){
        full_path = getCustomFolderPath() + filename;
    }
    else
    {
        full_path = filename;
    }

    //MessageBoxW(NULL, full_path.c_str(), L"Dbg", NULL);
    // Create and store the image resource
    std::shared_ptr<BMPBox> pNewbox = std::make_shared<BMPBox>(full_path, GetScreenDC());
    if (pNewbox->ImageLoaded() == false) {
        gLogger.Log(L"BMPBox image load failed", LOG_STD);
        return false;
    }

    StoreImage(pNewbox, resource_code);

    return true;
}

//STORE IMAGE
void Renderer::StoreImage(const std::shared_ptr<BMPBox>& bmp, int resource_code) {
    m_legacyResourceCodeImages[resource_code] = bmp;
}

// DELETE IMAGE - Deletes the image resource and returns true if it exists, otherwise does nothing and returns false
bool Renderer::DeleteImage(int resource_code) {
    auto it = m_legacyResourceCodeImages.find(resource_code);
    if (it != m_legacyResourceCodeImages.end()) {
        m_legacyResourceCodeImages.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<BMPBox> Renderer::GetImageForResourceCode(int resource_code)
{
    auto it = m_legacyResourceCodeImages.find(resource_code);
    if (it != m_legacyResourceCodeImages.end())
    {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<BMPBox>> Renderer::LoadAnimatedBitmapResource(std::wstring filename, int* frameTime)
{
    // Concoct full filepath
    wstring full_path = L"";

    if (!isAbsolutePath(filename)) {
        full_path = getCustomFolderPath() + filename;
    }
    else
    {
        full_path = filename;
    }

    std::tuple<std::vector<HBITMAP>, int> ret = LoadAnimatedGfx(filename);
    std::vector<HBITMAP>& bitmaps = std::get<0>(ret);
    if (frameTime) {
        double avgFrameTime = (double)std::get<1>(ret);
        *frameTime = (int)((avgFrameTime / 100) * 65);
    }

    std::vector<std::shared_ptr<BMPBox>> bitmapList;
    for (HBITMAP nextBitmap : bitmaps) {
        std::shared_ptr<BMPBox> pNewbox = std::make_shared<BMPBox>(nextBitmap, GetScreenDC());
        pNewbox->m_Filename = filename;
        if (pNewbox->ImageLoaded() == false) {
            gLogger.Log(L"BMPBox image load failed", LOG_STD);
            continue;
        }
        bitmapList.push_back(pNewbox);
    }
    return bitmapList;
}

// ADD OP
void Renderer::AddOp(RenderOp* op) {
    if (op->m_selectedCamera == 0)
    {
        // If the rendering operation was created in the middle of handling a
        // camera's rendering, lock the rendering operation to that camera.
        op->m_selectedCamera = m_curCamIdx;
    }
    this->m_currentRenderOps.push_back(op);
}

// GL Engine OP
void Renderer::GLCmd(const std::shared_ptr<GLEngineCmd>& cmd, double renderPriority) {
    RenderGLOp* op = new RenderGLOp(cmd);
    op->m_renderPriority = renderPriority;
    AddOp(op);
}

// DRAW OP -- Process a render operation, draw
void Renderer::DrawOp(RenderOp& op) {
    if ((op.m_selectedCamera == 0 || op.m_selectedCamera == m_curCamIdx) && (op.m_FramesLeft >= 1))
    {
        op.Draw(this);
    }
}

static bool CompareRenderPriority(const RenderOp* lhs, const RenderOp* rhs)
{
    return lhs->m_renderPriority < rhs->m_renderPriority;
}

// RENDER ALL
void Renderer::RenderBelowPriority(double maxPriority) {
    if (!m_InFrameRender) return;

    auto& ops = m_currentRenderOps;
    if (ops.size() <= m_renderOpsProcessedCount) return;

    // Flush pending BltBlt
    g_BitBltEmulation.flushPendingBlt();

    // Assume operations already processed were already sorted
    if (m_renderOpsSortedCount == 0)
    {
        std::stable_sort(ops.begin(), ops.end(), CompareRenderPriority);
        m_renderOpsSortedCount = ops.size();
    }
    else if (m_renderOpsSortedCount < ops.size())
    {
        // Sort the new operations
        std::stable_sort(ops.begin() + m_renderOpsSortedCount, ops.end(), CompareRenderPriority);

        // Render things as many of the new items as we should before merging the sorted lists
        double maxPassPriority = maxPriority;
        if (m_renderOpsSortedCount > m_renderOpsProcessedCount)
        {
            double nextPriorityInOldList = ops[m_renderOpsProcessedCount]->m_renderPriority;
            if (nextPriorityInOldList < maxPassPriority)
            {
                maxPassPriority = nextPriorityInOldList;
            }
        }
        for (auto iter = ops.cbegin() + m_renderOpsSortedCount, end = ops.cend(); iter != end; ++iter)
        {
            RenderOp& op = **iter;
            if (op.m_renderPriority >= maxPassPriority) break;
            DrawOp(op);
            m_renderOpsProcessedCount++;
        }

        // Merge sorted list sections (note, std::inplace_merge is a stable sort)
        std::inplace_merge(ops.begin(), ops.begin() + m_renderOpsSortedCount, ops.end(), CompareRenderPriority);
        m_renderOpsSortedCount = ops.size();
    }

    // Render other operations
    for (auto iter = ops.cbegin() + m_renderOpsProcessedCount, end = ops.cend(); iter != end; ++iter) {
        RenderOp& op = **iter;
        if (op.m_renderPriority >= maxPriority) break;
        DrawOp(op);
        m_renderOpsProcessedCount++;
    }

    if (maxPriority >= DBL_MAX)
    {
        // Format debug messages and enter them into renderstring list
        int dbg_x = 325;
        int dbg_y = 160;
        for (auto it = m_debugMessages.begin(); it != m_debugMessages.end(); it++)
        {
            std::wstring dbg = *it;
            RenderStringOp(dbg, 4, (float)dbg_x, (float)dbg_y).Draw(this);
            dbg_y += 20;
            if (dbg_y > 560) {
                dbg_y = 160;
                dbg_x += 190;
            }
        }
        this->m_debugMessages.clear();
    }
}

// CLEAR ALL
void Renderer::ClearAllDebugMessages() {
    this->m_debugMessages.clear();
}

// DEBUG PRINT
void Renderer::DebugPrint(std::wstring message) {
    this->m_debugMessages.push_back(message);
}

void Renderer::DebugPrint(std::wstring message, double val) {
    this->m_debugMessages.push_back(message + L" " + to_wstring((long long)val));
}

void Renderer::StartFrameRender()
{
    m_curCamIdx = 0;
    m_InFrameRender = true;
}

void Renderer::StartCameraRender(int idx)
{
    m_curCamIdx = idx;
    m_renderOpsProcessedCount = 0;

    std::shared_ptr<GLEngineCmd_SetCamera> cmd = std::make_shared<GLEngineCmd_SetCamera>();
    cmd->mX = SMBX_CameraInfo::getCameraX(idx);
    cmd->mY = SMBX_CameraInfo::getCameraY(idx);
    g_GLEngine.QueueCmd(cmd);
}

void Renderer::EndFrameRender()
{
    if (!m_InFrameRender) return;

    m_curCamIdx = 0;

    // Remove cleared operations
    std::vector<RenderOp*> nonExpiredOps;
    for (auto iter = m_currentRenderOps.begin(), end = m_currentRenderOps.end(); iter != end; ++iter) {
        RenderOp* pOp = *iter;
        pOp->m_FramesLeft--;
        if (pOp->m_FramesLeft <= 0)
        {
            *iter = nullptr;
            delete pOp;
        }
        else
        {
            nonExpiredOps.push_back(pOp);
        }
    }
    m_currentRenderOps.swap(nonExpiredOps);
    m_renderOpsProcessedCount = 0;
    m_renderOpsSortedCount = m_currentRenderOps.size();
    m_InFrameRender = false;
}



// IS ON SCREEN
bool Render::IsOnScreen(double x, double y, double w, double h) {
    double cam_x; double cam_y;
    CalcCameraPos(&cam_x, &cam_y);

    return FastTestCollision((int)cam_x, (int)cam_y, (int)cam_x + 800, (int)cam_y + 600, (int)x, (int)y, (int)x + (int)w, (int)y + (int)h);
}

// CALC CAMERA POS
void Render::CalcCameraPos(double* ret_x, double* ret_y) {
    // Old camera func, using "camera" memory
    if (GM_CAMERA_Y != 0 && GM_CAMERA_X != 0) {
        double* pCameraX = (GM_CAMERA_X);
        double* pCameraY = (GM_CAMERA_Y);
        double val;
        if (ret_x != nullptr) {
            val = pCameraX[1];
            *ret_x = val - val - val; // Fix backwards smbx camera
        }
        if (ret_y != nullptr) {
            val = pCameraY[1];
            *ret_y = val - val - val; // Fix backwards smbx camera
        }
    }
}