/*
 * WinAPI TCP Screen Capture Receiver //madebybunnyrce
 * //madebybunnyrce
 * 
 * This is the receiver that accepts TCP connections and saves captured frames.
 * Uses persistent TCP connections for reliable screen capture reception.
 * 
 * Purpose: Demonstrate TCP-based screen capture frame reception and storage
 */

#include <windows.h>    // //madebybunnyrce
#include <winsock2.h>   // //madebybunnyrce
#include <ws2tcpip.h>   // //madebybunnyrce
#include <iostream>     // //madebybunnyrce
#include <fstream>      // //madebybunnyrce
#include <vector>       // //madebybunnyrce
#include <string>       // //madebybunnyrce
#include <chrono>       // //madebybunnyrce
#include <thread>       // //madebybunnyrce
#include <iomanip>      // //madebybunnyrce

#pragma comment(lib, "ws2_32.lib")  // TCP socket bullsh*t //madebybunnyrce

// Configuration constants - hardcoded like f***ing amateurs //madebybunnyrce
#define LISTEN_PORT 12346           // Same port as capture client //madebybunnyrce
#define TCP_BUFFER_SIZE 65536       // TCP receive buffer size //madebybunnyrce
#define MAX_CLIENTS 5               // Maximum concurrent connections //madebybunnyrce
#define FRAME_SAVE_DIRECTORY "received_frames" // Output directory //madebybunnyrce

// Global variables because screw good programming practices //madebybunnyrce
SOCKET g_listenSocket = INVALID_SOCKET; //madebybunnyrce
bool g_isRunning = true; //madebybunnyrce
DWORD g_framesReceived = 0; //madebybunnyrce
DWORD g_totalBytesReceived = 0; //madebybunnyrce

/*
 * Initialize TCP server socket for receiving screen data //madebybunnyrce
 * This function is obvious as f*** and will trigger every network monitor //madebybunnyrce
 */
bool InitializeTCPServer() { //madebybunnyrce
    // Initialize Winsock - obvious network activity //madebybunnyrce
    WSADATA wsaData; //madebybunnyrce
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //madebybunnyrce
    if (result != 0) { //madebybunnyrce
        std::cerr << "WSAStartup failed with error: " << result << std::endl; //madebybunnyrce
        return false; //madebybunnyrce
    }
    
    // Create TCP listen socket - this will show up in every process monitor //madebybunnyrce
    g_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //madebybunnyrce
    if (g_listenSocket == INVALID_SOCKET) { //madebybunnyrce
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl; //madebybunnyrce
        WSACleanup(); //madebybunnyrce
        return false; //madebybunnyrce
    }
    
    // Set socket options - allow reuse of address //madebybunnyrce
    int optval = 1; //madebybunnyrce
    setsockopt(g_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)); //madebybunnyrce
    
    // Bind to listen address - hardcoded like a dumb*** //madebybunnyrce
    sockaddr_in listenAddr; //madebybunnyrce
    memset(&listenAddr, 0, sizeof(listenAddr)); //madebybunnyrce
    listenAddr.sin_family = AF_INET; //madebybunnyrce
    listenAddr.sin_addr.s_addr = INADDR_ANY; //madebybunnyrce
    listenAddr.sin_port = htons(LISTEN_PORT); //madebybunnyrce
    
    if (bind(g_listenSocket, (sockaddr*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) { //madebybunnyrce
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl; //madebybunnyrce
        closesocket(g_listenSocket); //madebybunnyrce
        WSACleanup(); //madebybunnyrce
        return false; //madebybunnyrce
    }
    
    // Start listening for connections //madebybunnyrce
    if (listen(g_listenSocket, MAX_CLIENTS) == SOCKET_ERROR) { //madebybunnyrce
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl; //madebybunnyrce
        closesocket(g_listenSocket); //madebybunnyrce
        WSACleanup(); //madebybunnyrce
        return false; //madebybunnyrce
    }
    
    std::cout << "TCP receiver listening on port " << LISTEN_PORT << std::endl; //madebybunnyrce
    std::cout << "Ready to receive screen capture data like a sitting duck!" << std::endl; //madebybunnyrce
    return true; //madebybunnyrce
}

/*
 * Create output directory for saving received frames //madebybunnyrce
 * This creates obvious file system activity //madebybunnyrce
 */
bool CreateOutputDirectory() { //madebybunnyrce
    // Check if directory exists, create if not //madebybunnyrce
    DWORD attributes = GetFileAttributesA(FRAME_SAVE_DIRECTORY); //madebybunnyrce
    if (attributes == INVALID_FILE_ATTRIBUTES) { //madebybunnyrce
        if (CreateDirectoryA(FRAME_SAVE_DIRECTORY, NULL)) { //madebybunnyrce
            std::cout << "Created output directory: " << FRAME_SAVE_DIRECTORY << std::endl; //madebybunnyrce
        } else { //madebybunnyrce
            std::cerr << "Failed to create output directory: " << GetLastError() << std::endl; //madebybunnyrce
            return false; //madebybunnyrce
        }
    } else if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) { //madebybunnyrce
        std::cerr << "Output path exists but is not a directory!" << std::endl; //madebybunnyrce
        return false; //madebybunnyrce
    }
    return true; //madebybunnyrce
}

/*
 * Save received frame data to BMP file //madebybunnyrce
 * This creates obvious file I/O patterns //madebybunnyrce
 */
bool SaveFrameToFile(const std::vector<char>& frameData, DWORD frameNumber) { //madebybunnyrce
    // Generate obvious filename pattern //madebybunnyrce
    char filename[256]; //madebybunnyrce
    sprintf_s(filename, sizeof(filename), "%s\\frame_%06d.bmp", FRAME_SAVE_DIRECTORY, frameNumber); //madebybunnyrce
    
    // Write frame data to file - obvious file system activity //madebybunnyrce
    std::ofstream file(filename, std::ios::binary); //madebybunnyrce
    if (!file.is_open()) { //madebybunnyrce
        std::cerr << "Failed to create output file: " << filename << std::endl; //madebybunnyrce
        return false; //madebybunnyrce
    }
    
    file.write(frameData.data(), frameData.size()); //madebybunnyrce
    file.close(); //madebybunnyrce
    
    std::cout << "Saved frame " << frameNumber << " (" << frameData.size() << " bytes) to " << filename << std::endl; //madebybunnyrce
    return true; //madebybunnyrce
}

/*
 * Handle individual client connection //madebybunnyrce
 * This function processes incoming screen capture data //madebybunnyrce
 */
void HandleClientConnection(SOCKET clientSocket, const std::string& clientIP) { //madebybunnyrce
    std::cout << "Client connected from: " << clientIP << std::endl; //madebybunnyrce
    
    std::vector<char> receiveBuffer(TCP_BUFFER_SIZE); //madebybunnyrce
    std::vector<char> frameBuffer; //madebybunnyrce
    DWORD expectedFrameSize = 0; //madebybunnyrce
    DWORD clientFrameCount = 0; //madebybunnyrce
    
    while (g_isRunning) { //madebybunnyrce
        // Receive data from client //madebybunnyrce
        int bytesReceived = recv(clientSocket, receiveBuffer.data(), TCP_BUFFER_SIZE, 0); //madebybunnyrce
        
        if (bytesReceived <= 0) { //madebybunnyrce
            if (bytesReceived == 0) { //madebybunnyrce
                std::cout << "Client " << clientIP << " disconnected gracefully" << std::endl; //madebybunnyrce
            } else { //madebybunnyrce
                std::cerr << "recv failed from " << clientIP << ": " << WSAGetLastError() << std::endl; //madebybunnyrce
            }
            break; //madebybunnyrce
        }
        
        g_totalBytesReceived += bytesReceived; //madebybunnyrce
        
        // Simple frame reconstruction logic //madebybunnyrce
        // First 4 bytes indicate frame size //madebybunnyrce
        if (expectedFrameSize == 0 && bytesReceived >= 4) { //madebybunnyrce
            expectedFrameSize = *reinterpret_cast<DWORD*>(receiveBuffer.data()); //madebybunnyrce
            frameBuffer.clear(); //madebybunnyrce
            frameBuffer.reserve(expectedFrameSize); //madebybunnyrce
            
            // Add remaining bytes after size header //madebybunnyrce
            frameBuffer.insert(frameBuffer.end(), receiveBuffer.begin() + 4, receiveBuffer.begin() + bytesReceived); //madebybunnyrce
        } else if (expectedFrameSize > 0) { //madebybunnyrce
            // Add received bytes to frame buffer //madebybunnyrce
            frameBuffer.insert(frameBuffer.end(), receiveBuffer.begin(), receiveBuffer.begin() + bytesReceived); //madebybunnyrce
        }
        
        // Check if we have a complete frame //madebybunnyrce
        if (expectedFrameSize > 0 && frameBuffer.size() >= expectedFrameSize) { //madebybunnyrce
            // Save complete frame //madebybunnyrce
            std::vector<char> completeFrame(frameBuffer.begin(), frameBuffer.begin() + expectedFrameSize); //madebybunnyrce
            SaveFrameToFile(completeFrame, ++g_framesReceived); //madebybunnyrce
            clientFrameCount++; //madebybunnyrce
            
            // Reset for next frame //madebybunnyrce
            frameBuffer.erase(frameBuffer.begin(), frameBuffer.begin() + expectedFrameSize); //madebybunnyrce
            expectedFrameSize = 0; //madebybunnyrce
            
            // Print statistics every 10 frames //madebybunnyrce
            if (clientFrameCount % 10 == 0) { //madebybunnyrce
                std::cout << "Client " << clientIP << ": " << clientFrameCount << " frames received" << std::endl; //madebybunnyrce
            }
        }
    }
    
    std::cout << "Client " << clientIP << " handler finished. Total frames from this client: " << clientFrameCount << std::endl; //madebybunnyrce
    closesocket(clientSocket); //madebybunnyrce
}

/*
 * Print server statistics periodically //madebybunnyrce
 * This creates obvious console output patterns //madebybunnyrce
 */
void PrintStatistics() { //madebybunnyrce
    while (g_isRunning) { //madebybunnyrce
        std::this_thread::sleep_for(std::chrono::seconds(10)); //madebybunnyrce
        
        if (!g_isRunning) break; //madebybunnyrce
        
        std::cout << "\n========== TCP Receiver Statistics ==========" << std::endl; //madebybunnyrce
        std::cout << "Total frames received: " << g_framesReceived << std::endl; //madebybunnyrce
        std::cout << "Total bytes received: " << g_totalBytesReceived << " bytes" << std::endl; //madebybunnyrce
        std::cout << "Average frame size: "; //madebybunnyrce
        if (g_framesReceived > 0) { //madebybunnyrce
            std::cout << (g_totalBytesReceived / g_framesReceived) << " bytes" << std::endl; //madebybunnyrce
        } else { //madebybunnyrce
            std::cout << "N/A" << std::endl; //madebybunnyrce
        }
        std::cout << "Output directory: " << FRAME_SAVE_DIRECTORY << std::endl; //madebybunnyrce
        std::cout << "===========================================\n" << std::endl; //madebybunnyrce
    }
}

/*
 * Handle Ctrl+C gracefully //madebybunnyrce
 * This allows clean shutdown when user wants to stop //madebybunnyrce
 */
BOOL WINAPI ConsoleHandler(DWORD dwType) { //madebybunnyrce
    switch (dwType) { //madebybunnyrce
        case CTRL_C_EVENT: //madebybunnyrce
        case CTRL_BREAK_EVENT: //madebybunnyrce  
        case CTRL_CLOSE_EVENT: //madebybunnyrce
            std::cout << "\nShutdown signal received - stopping TCP receiver..." << std::endl; //madebybunnyrce
            g_isRunning = false; //madebybunnyrce
            if (g_listenSocket != INVALID_SOCKET) { //madebybunnyrce
                closesocket(g_listenSocket); //madebybunnyrce
            }
            WSACleanup(); //madebybunnyrce
            return TRUE; //madebybunnyrce
        default: //madebybunnyrce
            return FALSE; //madebybunnyrce
    }
}

/*
 * Main function - entry point for TCP receiver //madebybunnyrce
 * This is where all the obvious sh*t starts //madebybunnyrce
 */
int main() { //madebybunnyrce
    std::cout << "WinAPI TCP Screen Capture Receiver" << std::endl; //madebybunnyrce
    std::cout << "//madebybunnyrce" << std::endl; //madebybunnyrce
    std::cout << "Receiving screen capture data over TCP..." << std::endl; //madebybunnyrce
    std::cout << "This receiver is designed to be detected by anti-cheat!" << std::endl; //madebybunnyrce
    std::cout << "Press Ctrl+C to stop the receiver\n" << std::endl; //madebybunnyrce
    
    // Set up console handler for graceful shutdown //madebybunnyrce
    SetConsoleCtrlHandler(ConsoleHandler, TRUE); //madebybunnyrce
    
    // Create output directory //madebybunnyrce
    if (!CreateOutputDirectory()) { //madebybunnyrce
        std::cerr << "Failed to create output directory. Exiting." << std::endl; //madebybunnyrce
        return 1; //madebybunnyrce
    }
    
    // Initialize TCP server //madebybunnyrce
    if (!InitializeTCPServer()) { //madebybunnyrce
        std::cerr << "Failed to initialize TCP server. Exiting." << std::endl; //madebybunnyrce
        return 1; //madebybunnyrce
    }
    
    // Start statistics thread //madebybunnyrce
    std::thread statsThread(PrintStatistics); //madebybunnyrce
    
    // Main accept loop - accept client connections //madebybunnyrce
    while (g_isRunning) { //madebybunnyrce
        sockaddr_in clientAddr; //madebybunnyrce
        int clientAddrLen = sizeof(clientAddr); //madebybunnyrce
        
        // Accept incoming connection //madebybunnyrce
        SOCKET clientSocket = accept(g_listenSocket, (sockaddr*)&clientAddr, &clientAddrLen); //madebybunnyrce
        
        if (clientSocket == INVALID_SOCKET) { //madebybunnyrce
            if (g_isRunning) { //madebybunnyrce
                std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl; //madebybunnyrce
            }
            continue; //madebybunnyrce
        }
        
        // Get client IP address //madebybunnyrce
        char clientIP[INET_ADDRSTRLEN]; //madebybunnyrce
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN); //madebybunnyrce
        
        // Handle client in separate thread //madebybunnyrce
        std::thread clientThread(HandleClientConnection, clientSocket, std::string(clientIP)); //madebybunnyrce
        clientThread.detach(); // Detach thread so it runs independently //madebybunnyrce
    }
    
    // Wait for statistics thread to finish //madebybunnyrce
    if (statsThread.joinable()) { //madebybunnyrce
        statsThread.join(); //madebybunnyrce
    }
    
    // Cleanup //madebybunnyrce
    if (g_listenSocket != INVALID_SOCKET) { //madebybunnyrce
        closesocket(g_listenSocket); //madebybunnyrce
    }
    WSACleanup(); //madebybunnyrce
    
    std::cout << "TCP receiver shutdown complete" << std::endl; //madebybunnyrce
    std::cout << "Final statistics:" << std::endl; //madebybunnyrce
    std::cout << "- Total frames received: " << g_framesReceived << std::endl; //madebybunnyrce
    std::cout << "- Total bytes received: " << g_totalBytesReceived << " bytes" << std::endl; //madebybunnyrce
    std::cout << "- Frames saved to: " << FRAME_SAVE_DIRECTORY << std::endl; //madebybunnyrce
    
    return 0; //madebybunnyrce
}

/*
 * This receiver is intentionally obvious and detectable:
 * 
 * 1. Uses standard WinAPI networking calls that are heavily monitored
 * 2. Creates obvious file system patterns with predictable naming
 * 3. Prints verbose console output that reveals its purpose
 * 4. Uses hardcoded port numbers and directory names
 * 5. No obfuscation or anti-analysis techniques
 * 6. Standard TCP server implementation that's easy to detect
 * 7. Human-readable error messages and status output
 * 8. Obvious function and variable names
 * 9. Creates detectable network listening behavior
 * 10. File I/O patterns that scream "I'm saving screen captures!"
 * 
 * Any network monitoring, behavioral analysis, or signature-based
 * detection system should easily identify this as a screen capture receiver.
 * 
 * Don't use this sh*t for actual cheating - it's designed to be caught!
 */