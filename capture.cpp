/*
 * WinAPI TCP Screen Capture
 * //madebybunnyrce
 * 
 * This is the main capture executable that grabs screen data and streams it over TCP.
 * Uses persistent TCP connections for reliable screen capture transmission.
 * 
 * Purpose: Demonstrate TCP-based screen capture streaming
 */

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")  // TCP socket bullsh*t
#pragma comment(lib, "gdi32.lib")   // Screen capture garbage
#pragma comment(lib, "user32.lib")  // More obvious WinAPI crap

// Configuration constants - hardcoded like f***ing amateurs
#define SERVER_IP "127.0.0.1"      // Localhost because we're obvious
#define SERVER_PORT 12346          // Different port from UDP version
#define CAPTURE_INTERVAL 100       // 100ms intervals - predictable as hell
#define TCP_BUFFER_SIZE 65536      // TCP send buffer size
#define BITMAP_HEADER_SIZE 54      // BMP header size because we're lazy

// Global variables because screw good programming practices
SOCKET g_tcpSocket = INVALID_SOCKET;
sockaddr_in g_serverAddr;
bool g_isCapturing = false;
bool g_isConnected = false;
int g_screenWidth = 0;
int g_screenHeight = 0;
DWORD g_framesSent = 0;
DWORD g_totalBytesSent = 0;

/*
 * Initialize the TCP socket for streaming screen data
 * This function creates persistent connections that are obvious as f***
 */
bool InitializeTCPSocket() {
    // Initialize Winsock - obvious network activity
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }
    
    // Create TCP socket - this will show up in every process monitor
    g_tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_tcpSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    
    // Set up server address - hardcoded like a dumb***
    memset(&g_serverAddr, 0, sizeof(g_serverAddr));
    g_serverAddr.sin_family = AF_INET;
    g_serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &g_serverAddr.sin_addr);
    
    std::cout << "[OBVIOUS] TCP socket initialized - any network monitor will see this sh*t" << std::endl;
    return true;
}

/*
 * Connect to TCP server - creates obvious connection patterns
 * This establishes persistent connections that are trivial to detect
 */
bool ConnectToServer() {
    std::cout << "[CONNECTING] Attempting TCP connection - this will be obvious as hell" << std::endl;
    
    // Connect to server - obvious three-way handshake
    int result = connect(g_tcpSocket, (sockaddr*)&g_serverAddr, sizeof(g_serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connection failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    
    g_isConnected = true;
    std::cout << "[DETECTED] TCP connection established - anti-cheat should flag this now" << std::endl;
    std::cout << "[WARNING] Persistent connection is easier to monitor than UDP" << std::endl;
    
    return true;
}

/*
 * Get screen dimensions using obvious WinAPI calls
 * This function screams "I'M CAPTURING THE SCREEN!"
 */
void GetScreenDimensions() {
    // Get screen dimensions - super f***ing obvious
    g_screenWidth = GetSystemMetrics(SM_CXSCREEN);   // RED FLAG
    g_screenHeight = GetSystemMetrics(SM_CYSCREEN);  // ANOTHER RED FLAG
    
    std::cout << "[DETECTED] Screen dimensions: " << g_screenWidth << "x" << g_screenHeight << std::endl;
    std::cout << "[WARNING] GetSystemMetrics() calls are monitored by anti-cheat" << std::endl;
}

/*
 * Capture screen using the most obvious WinAPI method possible
 * This function will trigger every API monitor in existence
 */
std::vector<BYTE> CaptureScreen() {
    // Get desktop device context - obvious as hell
    HDC screenDC = GetDC(NULL);  // MAJOR RED FLAG - monitored by everything
    if (!screenDC) {
        std::cerr << "Failed to get screen DC - this sh*t is broken" << std::endl;
        return std::vector<BYTE>();
    }
    
    // Create compatible DC - another obvious call
    HDC memoryDC = CreateCompatibleDC(screenDC);  // DETECTED BY ALL ANTI-CHEAT
    if (!memoryDC) {
        std::cerr << "Failed to create compatible DC" << std::endl;
        ReleaseDC(NULL, screenDC);
        return std::vector<BYTE>();
    }
    
    // Create bitmap - more obvious bullsh*t
    HBITMAP bitmap = CreateCompatibleBitmap(screenDC, g_screenWidth, g_screenHeight);
    if (!bitmap) {
        std::cerr << "Failed to create bitmap" << std::endl;
        DeleteDC(memoryDC);
        ReleaseDC(NULL, screenDC);
        return std::vector<BYTE>();
    }
    
    // Select bitmap into memory DC
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDC, bitmap);
    
    // Copy screen to bitmap - THE MOST OBVIOUS CALL EVER
    if (!BitBlt(memoryDC, 0, 0, g_screenWidth, g_screenHeight, screenDC, 0, 0, SRCCOPY)) {
        std::cerr << "BitBlt failed - screen capture sh*t broke" << std::endl;
        SelectObject(memoryDC, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memoryDC);
        ReleaseDC(NULL, screenDC);
        return std::vector<BYTE>();
    }
    
    // Get bitmap data - more obvious API calls
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = g_screenWidth;
    bi.biHeight = -g_screenHeight;  // Negative for top-down bitmap
    bi.biPlanes = 1;
    bi.biBitCount = 24;  // 24-bit RGB because we're predictable
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    
    // Calculate image size
    int imageSize = ((g_screenWidth * 3 + 3) & ~3) * g_screenHeight;
    std::vector<BYTE> imageData(BITMAP_HEADER_SIZE + imageSize);
    
    // Create BMP header - because we're too lazy for compression
    BITMAPFILEHEADER bf;
    bf.bfType = 0x4D42;  // "BM"
    bf.bfSize = BITMAP_HEADER_SIZE + imageSize;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits = BITMAP_HEADER_SIZE;
    
    // Copy headers to buffer
    memcpy(&imageData[0], &bf, sizeof(BITMAPFILEHEADER));
    memcpy(&imageData[sizeof(BITMAPFILEHEADER)], &bi, sizeof(BITMAPINFOHEADER));
    
    // Get bitmap bits - final obvious API call
    if (!GetDIBits(screenDC, bitmap, 0, g_screenHeight, &imageData[BITMAP_HEADER_SIZE], (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
        std::cerr << "GetDIBits failed - this capture sh*t is f***ed" << std::endl;
        imageData.clear();
    }
    
    // Clean up resources like we should have done from the start
    SelectObject(memoryDC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memoryDC);
    ReleaseDC(NULL, screenDC);
    
    return imageData;
}

/*
 * Send image data over TCP stream
 * This function creates obvious persistent data flow patterns
 */
bool SendImageData(const std::vector<BYTE>& imageData) {
    if (!g_isConnected || imageData.empty()) {
        std::cerr << "Not connected or no image data to send, you dumb***" << std::endl;
        return false;
    }
    
    // Create frame header for TCP stream
    struct FrameHeader {
        DWORD magic;           // Magic number for validation
        DWORD frameNumber;     // Frame sequence number
        DWORD dataSize;        // Size of frame data
        DWORD timestamp;       // Timestamp
    };
    
    FrameHeader header;
    header.magic = 0xDEADBEEF;  // Obvious magic number
    header.frameNumber = g_framesSent;
    header.dataSize = (DWORD)imageData.size();
    header.timestamp = GetTickCount();
    
    // Send header first - obvious protocol pattern
    int result = send(g_tcpSocket, (char*)&header, sizeof(FrameHeader), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to send frame header: " << WSAGetLastError() << std::endl;
        g_isConnected = false;
        return false;
    }
    
    // Send image data in chunks - obvious streaming pattern
    const char* dataPtr = (char*)&imageData[0];
    int totalSize = (int)imageData.size();
    int totalSent = 0;
    
    while (totalSent < totalSize) {
        int chunkSize = min(TCP_BUFFER_SIZE, totalSize - totalSent);
        
        result = send(g_tcpSocket, dataPtr + totalSent, chunkSize, 0);
        if (result == SOCKET_ERROR) {
            std::cerr << "Failed to send frame data: " << WSAGetLastError() << std::endl;
            g_isConnected = false;
            return false;
        }
        
        totalSent += result;
        g_totalBytesSent += result;
    }
    
    g_framesSent++;
    std::cout << "[STREAMING] Frame " << g_framesSent << " sent over TCP (" << totalSize << " bytes)" << std::endl;
    return true;
}

/*
 * Main capture loop - runs continuously and obviously
 * This creates persistent TCP streaming that's easy to detect
 */
void CaptureLoop() {
    std::cout << "[STARTING] TCP screen capture loop - network monitoring should detect this now" << std::endl;
    std::cout << "[WARNING] Persistent TCP streaming is more obvious than UDP bursts" << std::endl;
    
    auto lastCapture = std::chrono::high_resolution_clock::now();
    
    while (g_isCapturing && g_isConnected) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCapture);
        
        // Capture at fixed intervals - predictable as hell
        if (elapsed.count() >= CAPTURE_INTERVAL) {
            // Capture screen with obvious API calls
            std::vector<BYTE> imageData = CaptureScreen();
            if (!imageData.empty()) {
                // Send over TCP - obvious streaming patterns
                if (!SendImageData(imageData)) {
                    std::cerr << "TCP connection lost, stopping capture" << std::endl;
                    break;
                }
            }
            
            lastCapture = now;
            
            // Print obvious status every 50 frames
            if (g_framesSent % 50 == 0) {
                std::cout << "[STATUS] Frames streamed: " << g_framesSent 
                         << ", Total bytes: " << g_totalBytesSent << std::endl;
                std::cout << "[REMINDER] TCP streaming is obvious as f*** and will be detected" << std::endl;
            }
        }
        
        // Small sleep to prevent 100% CPU usage (still obvious though)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "[STOPPED] TCP capture loop ended" << std::endl;
}

/*
 * Cleanup function - close sockets and sh*t
 */
void Cleanup() {
    if (g_tcpSocket != INVALID_SOCKET) {
        // Graceful shutdown - obvious connection closure
        shutdown(g_tcpSocket, SD_BOTH);
        closesocket(g_tcpSocket);
        g_tcpSocket = INVALID_SOCKET;
    }
    WSACleanup();
    g_isConnected = false;
    std::cout << "[CLEANUP] TCP resources cleaned up" << std::endl;
}

/*
 * Main function - entry point for this obvious TCP bullsh*t
 */
int main() {
    std::cout << "=== WinAPI TCP Screen Capture - Persistent Connection Edition ===" << std::endl;
    std::cout << "WARNING: This tool uses TCP connections that are easier to detect than UDP" << std::endl;
    std::cout << "Don't use this sh*t for actual cheating, persistent connections get caught immediately" << std::endl;
    std::cout << "=================================================================" << std::endl;
    
    // Initialize TCP socket
    if (!InitializeTCPSocket()) {
        std::cerr << "Failed to initialize TCP socket - this sh*t is broken" << std::endl;
        return 1;
    }
    
    // Get screen dimensions
    GetScreenDimensions();
    if (g_screenWidth == 0 || g_screenHeight == 0) {
        std::cerr << "Invalid screen dimensions - something's f***ed" << std::endl;
        Cleanup();
        return 1;
    }
    
    // Connect to server
    if (!ConnectToServer()) {
        std::cerr << "Failed to connect to TCP server - check if receiver is running" << std::endl;
        Cleanup();
        return 1;
    }
    
    std::cout << "Press ENTER to start TCP streaming (Ctrl+C to stop)..." << std::endl;
    std::cin.get();
    
    // Start capture loop
    g_isCapturing = true;
    std::thread captureThread(CaptureLoop);
    
    std::cout << "TCP streaming... Press ENTER to stop" << std::endl;
    std::cin.get();
    
    // Stop capturing
    g_isCapturing = false;
    if (captureThread.joinable()) {
        captureThread.join();
    }
    
    std::cout << "=== Final TCP Statistics ===" << std::endl;
    std::cout << "Frames streamed: " << g_framesSent << std::endl;
    std::cout << "Total bytes sent: " << g_totalBytesSent << std::endl;
    std::cout << "Average bytes per frame: " << (g_framesSent > 0 ? g_totalBytesSent / g_framesSent : 0) << std::endl;
    std::cout << "TCP connection was obvious as f*** and definitely got detected" << std::endl;
    
    Cleanup();
    return 0;
}