#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

const int n1 = 2;
const int n2 = 1;
const int n3 = 0;
const int n4 = 2;
const int N = 10;

void arrow(HDC hdc, double fi, int px, int py);

float** randm(int n);
float** mulmr(double c, float** mat, int n);

char** setVertexes(int n);
int* setCoordsX(int n, int start);
int* setCoordsY(int n, int start);

int mod(int x1, int x2);

float **createAdjMatrix(float** A, double c, int n, bool isDirected);

void drawDirectedGraph(HDC hdc, float** matrix, int n, char **nn, int *nx, int *ny, int startX, int startY);
void drawUndirectedGraph(HDC hdc, float** matrix, int n, char **nn, int *nx, int *ny, int startX, int startY);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS w;

    w.lpszClassName = L"Lab 3";
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;

    if(!RegisterClass(&w))
        return 0;

    HWND hWnd;
    MSG lpMsg;

    hWnd = CreateWindow(L"Lab 3",
                        "Lab 3 by Halai Andrii",
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        100,
                        100,
                        1200,
                        700,
                        NULL,
                        NULL,
                        (HINSTANCE)hInstance,
                        NULL);

    ShowWindow(hWnd, nCmdShow);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch(messg) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            char **nn;
            int *nx;
            int *ny;
            float **A;
            float **T;

            A = createAdjMatrix(A, (1.0 - n3*0.01 - n4*0.01 - 0.3), N, true);
            printf("Directed graph:\n");
            drawDirectedGraph(hdc, A, N, nn, nx, ny, 100, 100);
            printf("\n");

            T = createAdjMatrix(T, (1.0 - n3*0.01 - n4*0.01 - 0.3), N, false);
            printf("Undirected graph:\n");
            drawUndirectedGraph(hdc, A, N, nn, nx, ny, 600, 100);

            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
}

void arrow(HDC hdc, double fi, int px, int py) {
    fi = 3.1415*(180.0 - fi)/180.0;
    int lx,ly,rx,ry;
    lx = ceil(px+15*cos(fi+0.3));
    rx = ceil(px+15*cos(fi-0.3));
    ly = ceil(py+15*sin(fi+0.3));
    ry = ceil(py+15*sin(fi-0.3));
    MoveToEx(hdc, lx, ly, NULL);
    LineTo(hdc, px, py);
    LineTo(hdc, rx, ry);
}

float** randm(int n) {
    srand(2102);
    float** arr = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        arr[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            arr[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }
    return arr;
}

float** mulmr(double c, float** mat, int n) {
    float** res = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        res[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            res[i][j] = floor(mat[i][j] * c);
        }
    }
    return res;
}

char** setVertexes(int n) {
    char** arr = malloc(n * sizeof(char*));
    for(int i = 0; i < n; i++) {
        arr[i] = malloc(3 * sizeof(char));
        sprintf(arr[i], "%d", i+1);
    }
    return arr;
}

int* setCoordsX(int n, int start) {
    int *nx = malloc(n * sizeof(int));
    int vertSide = 2;
    int horizontalSide = (int) ceilf((float) n/2 - vertSide);
    if (horizontalSide > 1) {
        for (int i = 0; i < horizontalSide; i++) {
            nx[i] = start + i*100;
        }
        for (int i = horizontalSide; i < horizontalSide + vertSide; i++) {
            nx[i] = nx[i-1];
        }
        for (int i = horizontalSide+vertSide; i < 2*horizontalSide + vertSide; i++) {
            int j = 2*horizontalSide + vertSide;
            nx[i] = nx[j - i - 1];
        }
        for (int i = 2*horizontalSide + vertSide; i < n; i ++) {
            nx[i] = nx[i-1];
        }
    } else {
        int side = (int) ceilf((float)n/2);
        for (int i = 0; i < side; i++) {
            nx[i] = start + i*100;
        }
        for (int i = 1; i < n-side+1; i++) {
            nx[side+i-1] = nx[side - i];
        }
    }
    return nx;
}

int* setCoordsY(int n, int start) {
    int *ny = malloc(n * sizeof(int));
    int vertSide = 2;
    int horizontalSide = (int) ceilf((float) n/2 - vertSide);
    if (horizontalSide > 1) {
        for (int i = 0; i < horizontalSide; i++) {
            ny[i] = start;
        }
        for (int i = horizontalSide; i < horizontalSide + vertSide; i++) {
            ny[i] = ny[i-1] + 100;
        }
        for (int i = horizontalSide+vertSide; i < 2*horizontalSide + vertSide; i++) {
            ny[i] = ny[horizontalSide-1] + 300;
        }
        for (int i = 2*horizontalSide + vertSide; i < n; i ++) {
            ny[i] = ny[i-1] - 100;
        }
    } else {
        int side = (int) ceilf((float)n/2);
        for (int i = 0; i < side; i++) {
            ny[i] = start;
        }
        for (int i = side; i < n; i++) {
            ny[i] = start+100;
        }
    }
    return ny;
}

int mod(int x1, int x2) {
    int res = x1 - x2;
    if (res > 0) return res;
    else return (-1)*res;
}

float **createAdjMatrix(float** A, double c, int n, bool isDirected) {
    A = randm(2102);
    A = mulmr((1.0 - n3*0.01 - n4*0.01 - 0.3), A, n);
    if (!isDirected) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (A[i][j] == 1) {
                    A[j][i] = 1;
                }
            }
        }
    }
    return A;
}

void drawDirectedGraph(HDC hdc, float** matrix, int n, char **nn, int *nx, int *ny, int startX, int startY) {
    int vertSide = 2;
    int horizontalSide = (int) ceilf((float) n/2 - vertSide);
    if (horizontalSide <= 1) {
        horizontalSide = ceilf((float)n / 2);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.0f ", matrix[i][j]);
        }
        printf("\n");
    }

    nn = setVertexes(n);
    nx = setCoordsX(n, startX);
    ny = setCoordsY(n, startY);
    int dx = 16, dy = 16, dtx = 5;

    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

    SelectObject(hdc, KPen);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((matrix[i][j] == 1) && (ny[i] == ny[j]) && (mod(i,j) < horizontalSide) && (mod(nx[i], nx[j]) > 100)) { //horzntl arc
                if ((nx[i] - nx[j]) < 0) {
                    int xPoint = (nx[i] + nx[j]) / 2;
                    int yPoint = ((ny[i] + ny[j]) / 2) + 40;
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, xPoint, yPoint);
                    MoveToEx(hdc, xPoint, yPoint, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                    double leg = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]- ny[j], 2));
                    double angle = acos(leg/hypot)*180/3.1415;
                    int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                    int x1 = ceil(dx*cos(angle* (3.1415 / 180)));
                    arrow(hdc, angle, nx[j]-x1, ny[j]+y1);
                } else if((nx[i] - nx[j]) > 0) {
                    int xPoint = (nx[i] + nx[j]) / 2;
                    int yPoint = ((ny[i] + ny[j]) / 2) - 40;
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, xPoint, yPoint);
                    MoveToEx(hdc, xPoint, yPoint, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                    double leg = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]- ny[j], 2));
                    double angle = acos(leg/hypot)*180/3.1415;
                    int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                    int x1 = ceil(dx*cos(angle* (3.1415 / 180)));
                    arrow(hdc, angle+180, nx[j]+x1, ny[j]-y1);
                }
            } else if (matrix[i][j] == 1 && nx[i] != nx[j]) { // horzntl line
                if ((i >= j) && matrix[j][i] == 1) { //draw curve line
                    int xPoint = (nx[i] + nx[j]) / 2;
                    int yPoint = ((ny[i] + ny[j]) / 2) - 30;
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, xPoint, yPoint);
                    MoveToEx(hdc, xPoint, yPoint, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                    double leg = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]- ny[j], 2));
                    double angle = acos(leg/hypot)*180/3.1415;
                    int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                    int x1 = ceil(dx*cos(angle* (3.1415 / 180)));

                    if ((nx[i] - nx[j]) < 0) { //arrow for curve line
                        if (yPoint < ny[j] && xPoint < nx[j]) {
                            arrow(hdc, -1*angle, nx[j]-x1, ny[j]-y1);
                        } else if (yPoint > ny[j] && xPoint < nx[j]) {
                            arrow(hdc, angle, nx[j]-x1, ny[j]+y1);
                        }
                    } else if ((nx[i] - nx[j]) > 0) {
                        if (yPoint < ny[j] && xPoint > nx[j]) {
                            arrow(hdc, angle+180, nx[j]+x1, ny[j]-y1);
                        } else if (yPoint > ny[j] && xPoint > nx[j]) {
                            arrow(hdc, -1*angle+180, nx[j]+x1, ny[j]+y1);
                        }
                    }

                } else { //draw straight line
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    double hypot = sqrt(pow(nx[j]-nx[i], 2) + pow(ny[j]-ny[i], 2));
                    double leg = sqrt(pow(nx[j]-nx[i], 2) + pow(ny[j]- ny[j], 2));
                    double angle = acos(leg/hypot)*180/3.1415;
                    int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                    int x1 = ceil(dx*cos(angle* (3.1415 / 180)));
                    if (ny[i] == ny[j]) { //arrow
                        int direction = nx[i] - nx[j];
                        if (direction < 0) arrow(hdc, 0, nx[j]-dx, ny[j]);
                        else if (direction > 0) arrow(hdc, 180, nx[j]+dx, ny[j]);
                    } else if ((nx[i] - nx[j]) < 0) { // arrow between levels from left to right
                        if (ny[i] < ny[j]) {
                            arrow(hdc, -1*angle, nx[j]-x1, ny[j]-y1);
                        } else if (ny[i] > ny[j]) {
                            arrow(hdc, angle, nx[j]-x1, ny[j]+y1);
                        }
                    } else if ((nx[i] - nx[j]) > 0) { // arrow between levels from right to left
                        if (ny[i] < ny[j]) {
                            arrow(hdc, angle+180, nx[j]+x1, ny[j]-y1);
                        } else if (ny[i] > ny[j]) {
                            arrow(hdc, -1*angle+180, nx[j]+x1, ny[j]+y1);
                        }
                    }
                }
            } else if ((matrix[i][j] == 1) && (nx[i] == nx[j]) && (ny[i] != ny[j]) && (mod(ny[i],ny[j]) > 100)) { //vert arc
                if (mod(i, j) > 4 && nx[j] != 100 && nx[i] != 100) { // draw line if it is not same side
                    if ((i >= j) && matrix[j][i] == 1) { //draw curve line
                        int xPoint = (nx[i] + nx[j]) / 2 + 15;
                        int yPoint = ((ny[i] + ny[j]) / 2);
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, xPoint, yPoint);
                        MoveToEx(hdc, xPoint, yPoint, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                        double leg = sqrt(pow(nx[j]-nx[j], 2) + pow(ny[j]-yPoint, 2));
                        double angle = acos(leg/hypot)*180/3.1415;
                        int y1 = ceil(dx*cos(angle* (3.1415 / 180)));
                        int x1 = ceil(dx*sin(angle* (3.1415 / 180)));
                        if (ny[i] > ny[j]) { //arrow for vert curve line
                            arrow(hdc, angle+90, nx[j]+x1, ny[j]+y1);
                        } else if (ny[i] < ny[j]) {
                            arrow(hdc, -1*angle-90, nx[j]+x1, ny[j]-y1);
                        }
                    } else { //draw straight line
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        if (ny[i] < ny[j]) { //arrow for vert line
                            arrow(hdc, -90, nx[j], ny[j]-dy);
                        } else if (ny[i] > ny[j]) {
                            arrow(hdc, 90, nx[j], ny[j]+dy);
                        }
                    }
                } else { // arc if it is the same side
                    if (ny[i] < ny[j]) {
                        int xPoint = (nx[i] + nx[j]) / 2 + 40;
                        int yPoint = ((ny[i] + ny[j]) / 2);
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, xPoint, yPoint);
                        MoveToEx(hdc, xPoint, yPoint, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                        double leg = sqrt(pow(nx[j]-nx[j], 2) + pow(ny[j]-yPoint, 2));
                        double angle = acos(leg/hypot)*180/3.1415;
                        int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                        int x1 = ceil(dx*cos(angle* (3.1415 / 180)));
                        arrow(hdc, -90-angle, nx[j]+y1, ny[j]-x1);
                    } else if (ny[i] > ny[j]) {
                        int xPoint = (nx[i] + nx[j]) / 2 - 40;
                        int yPoint = ((ny[i] + ny[j]) / 2);
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, xPoint, yPoint);
                        MoveToEx(hdc, xPoint, yPoint, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                        double leg = sqrt(pow(nx[j]-nx[j], 2) + pow(ny[j]-yPoint, 2));
                        double angle = acos(leg/hypot)*180/3.1415;
                        int y1 = ceil(dx*sin(angle* (3.1415 / 180)));
                        int x1 = ceil(dx*cos(angle* (3.1415 / 180)));
                        arrow(hdc, 90-angle, nx[j]-y1, ny[j]+x1);
                    }
                }
            } else if (matrix[i][j] == 1 && ny[i] != ny[j]) { // vert line
                if ((i >= j) && matrix[j][i] == 1) { //draw curve line
                    int xPoint = (nx[i] + nx[j]) / 2 + 15;
                    int yPoint = ((ny[i] + ny[j]) / 2);
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, xPoint, yPoint);
                    MoveToEx(hdc, xPoint, yPoint, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    double hypot = sqrt(pow(nx[j]-xPoint, 2) + pow(ny[j]-yPoint, 2));
                    double leg = sqrt(pow(nx[j]-nx[j], 2) + pow(ny[j]-yPoint, 2));
                    double angle = acos(leg/hypot)*180/3.1415;
                    int y1 = ceil(dx*cos(angle* (3.1415 / 180)));
                    int x1 = ceil(dx*sin(angle* (3.1415 / 180)));
                    if (ny[i] > ny[j]) { //arrow for vert curve line
                        arrow(hdc, angle+90, nx[j]+x1, ny[j]+y1);
                    } else if (ny[i] < ny[j]) {
                        arrow(hdc, -1*angle-90, nx[j]+x1, ny[j]-y1);
                    }
                } else { //draw straight line
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j], ny[j]);
                    if (ny[i] < ny[j]) { //arrow for vert line
                        arrow(hdc, -90, nx[j], ny[j]-dy);
                    } else if (ny[i] > ny[j]) {
                        arrow(hdc, 90, nx[j], ny[j]+dy);
                    }
                }
            } else if (matrix[i][j] == 1 && i == j) { // loop
                int x1 = 25;
                int y1 = 2*x1;
                Arc(hdc, nx[i]-x1, ny[i]-y1, nx[j]+x1, ny[j], nx[i], ny[i], nx[j], ny[j]);
                arrow(hdc, -50, nx[i]-dx+1, ny[i]-6);
            }
        }
    }

    SelectObject(hdc, BPen);

    for (int i = 0; i < n; i++) {
        Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
        if (strlen(nn[i]) < 2) {
            TextOut(hdc, nx[i] - dtx, ny[i] - dy / 2, nn[i], 1);
        } else {
            TextOut(hdc, nx[i] - dtx-4, ny[i] - dy / 2, nn[i], 2);
        }
    }
}

void drawUndirectedGraph(HDC hdc, float** matrix, int n, char **nn, int *nx, int *ny, int startX, int startY) {
    int vertSide = 2;
    int horizontalSide = (int) ceilf((float) n/2 - vertSide);
    if (horizontalSide <= 1) {
        horizontalSide = ceilf((float)n / 2);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                matrix[j][i] = 1;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.0f ", matrix[i][j]);
        }
        printf("\n");
    }

    nn = setVertexes(n);
    nx = setCoordsX(n, startX);
    ny = setCoordsY(n, startY);
    int dx = 16, dy = 16, dtx = 5;

    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

    SelectObject(hdc, KPen);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((matrix[i][j] == 1) && (ny[i] == ny[j]) && (mod(i,j) < horizontalSide) && (mod(nx[i], nx[j]) > 100)) { //horzntl arc
                int xPoint = (nx[i] + nx[j]) / 2;
                int yPoint = ((ny[i] + ny[j]) / 2) + 40;
                MoveToEx(hdc, nx[i], ny[i], NULL);
                LineTo(hdc, xPoint, yPoint);
                MoveToEx(hdc, xPoint, yPoint, NULL);
                LineTo(hdc, nx[j], ny[j]);
            } else if (matrix[i][j] == 1 && nx[i] != nx[j]) { // horzntl line
                MoveToEx(hdc, nx[i], ny[i], NULL);
                LineTo(hdc, nx[j], ny[j]);
            } else if ((matrix[i][j] == 1) && (nx[i] == nx[j]) && (ny[i] != ny[j]) && (mod(ny[i],ny[j]) > 100)) { //vert arc
                if (mod(i, j) > 4 && nx[j] != startX && nx[i] != 100) { // draw line if it is not same side
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j], ny[j]);
                } else { // arc if it is the same side
                    int xPoint = (nx[i] + nx[j]) / 2 + 40;
                    int yPoint = ((ny[i] + ny[j]) / 2);
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, xPoint, yPoint);
                    MoveToEx(hdc, xPoint, yPoint, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            } else if (matrix[i][j] == 1 && ny[i] != ny[j]) { // vert line
                MoveToEx(hdc, nx[i], ny[i], NULL);
                LineTo(hdc, nx[j], ny[j]);
            } else if (matrix[i][j] == 1 && i == j) { // loop
                int x1 = 25;
                int y1 = 2*x1;
                Arc(hdc, nx[i]-x1, ny[i]-y1, nx[j]+x1, ny[j], nx[i], ny[i], nx[j], ny[j]);
            }
        }
    }

    SelectObject(hdc, BPen);

    for (int i = 0; i < n; i++) {
        Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
        if (strlen(nn[i]) < 2) {
            TextOut(hdc, nx[i] - dtx, ny[i] - dy / 2, nn[i], 1);
        } else {
            TextOut(hdc, nx[i] - dtx-4, ny[i] - dy / 2, nn[i], 2);
        }
    }
}
