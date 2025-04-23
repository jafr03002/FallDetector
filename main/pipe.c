#include <stdio.h>
#include <windows.h>

int pipe() {
    const char* pipeName = "\\\\.\\pipe\\my_pipe";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE | PIPE_WAIT,
        1, 512, 512,
        0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Failed to create pipe. Error: %d\n", GetLastError());
        return 1;
    }

    printf("Waiting for Python client to connect...\n");
    ConnectNamedPipe(hPipe, NULL);

    for (int i = 0; i < 10; i++) {
        char message[100];
        snprintf(message, sizeof(message), "%.2f,%.2f,%.2f\n", 0.01f * i, -0.98f, 9.81f);
        DWORD bytesWritten;
        WriteFile(hPipe, message, strlen(message), &bytesWritten, NULL);
        Sleep(500);
    }

    CloseHandle(hPipe);
    return 0;
}
