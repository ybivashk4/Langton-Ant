/**************************
 * Includes
 *
 **************************/

#include <gl/gl.h>
#include <stdio.h>
typedef struct{
	int rule; /* rule now 2: L(+90), 1: R(-90), 3: N(0), 4: U(180)*/
	int x;
	int y;
	
}Ant;

typedef struct{
	int color[20][3]; /* rgb color for rule (by index)*/
	int rule[20]; /* rule  - 2: L(+90), 1: R(-90), 3: N(0), 4: U(180)*/
	int count_rule;	
	int px;
	int iteration;
}Field;




/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);





void load_file_ants(Ant *ants, int *n){
	FILE *f;
	int m;
	if (ants){
		f = fopen("menu/files/ants.dat", "rb");
		if (f){
			fread(n, sizeof(*n), 1, f);
			*n = fread(ants, sizeof(*ants), 40, f);
			
			fclose(f);
		}else
			return;
	}else
		return;
}
	
void load_file_field(Field *field){
	FILE *f;
	if (field){
		f = fopen("menu/files/field.dat", "rb");
		if (f){
			fread(field, sizeof(*field), 1, f);
			fclose(f);
		}else
			return;
	}else
		return;
}

void del_ant(Ant *ants, int i, int *n){
	if (ants == NULL){
		return;
	}else if (*n == 0){
		return;
	}
	if (i < 0 || i > *n - 1){
		return;
	}
	Ant temp;
	while (i != *n - 1){
		temp = ants[i+1];
		ants[i+1] = ants[i];
		ants[i] = temp;
		i++;
	}
	(*n)--;
}



/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
	Ant ants[40] = {0};	
	Field field;
	char color[1000][720] = {0};
	int iter = 0, n = 0, j=0, width=0, height=0, i, flag=1;
    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);
	
    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 1000, 720,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
	
    /* program main loop */
    
    load_file_field(&field);
	load_file_ants(ants, &n);
	/* после завершения сразу закрывается, координаты */
	width = 1000 / field.px;
	height = 720 / field.px;
	for (i=0;i<n;i++){
		ants[i].x /= field.px;
		ants[i].y /= field.px;
	}
    
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
    
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
			int x=0;
			int y=0;
			
            glPushMatrix ();
        	
			color[ants[j].x][ants[j].y] = (color[ants[j].x][ants[j].y] + 1) % field.count_rule;
            if (flag){
            	for (x=0;x<width;x++)
	            	for(y=0;y<height;y++){
	            			glBegin (GL_QUADS);
								glColor3f (field.color[color[x][y]][0], field.color[color[x][y]][1], field.color[color[x][y]][2]); 
								glVertex2f (1 + x, y);
								
								glColor3f (field.color[color[x][y]][0], field.color[color[x][y]][1], field.color[color[x][y]][2]); 
								glVertex2f (x, y);
								
								glColor3f (field.color[color[x][y]][0], field.color[color[x][y]][1], field.color[color[x][y]][2]);
								glVertex2f (x, 1 + y);
								
								glColor3f (field.color[color[x][y]][0], field.color[color[x][y]][1], field.color[color[x][y]][2]); 
								glVertex2f (1 + x, 1 + y);
							glEnd ();
	            	}
            }
            
            		
			switch (ants[j].rule){
				case 1:
					switch(field.rule[color[ants[j].x][ants[j].y]]){
						case 1: 
							ants[j].y -= 1;
							ants[j].rule = 4;
							break;
						case 2:
							ants[j].y += 1  ;
							ants[j].rule = 3;
							break;
						case 3:
							ants[j].x -= 1  ;
							ants[j].rule = 1;
							break;
						case 4:
							ants[j].x += 1  ;
							ants[j].rule = 2;
							break;
						}
					break;
				case 2:
					switch(field.rule[color[ants[j].x][ants[j].y]]){
						case 1:
							ants[j].y += 1  ;
							ants[j].rule = 3;
							break;
						case 2:
							ants[j].y -= 1  ;
							ants[j].rule = 4;
							break;
						case 3:
							ants[j].x += 1  ;
							ants[j].rule = 2;
							break;
						case 4:
							ants[j].x -= 1  ;
							ants[j].rule = 1;
							break;
						}
					break;
				case 3:
					switch(field.rule[color[ants[j].x][ants[j].y]]){
						case 1:
							ants[j].x -= 1  ;
							ants[j].rule = 1;
							break;
						case 2:
							ants[j].x += 1  ;
							ants[j].rule = 2;
							break;
						case 3:
							ants[j].y += 1  ;
							ants[j].rule = 3;
							break;
						case 4:
							ants[j].y -= 1  ;
							ants[j].rule = 4;
							break;
						}
					break;
				case 4:
					switch(field.rule[color[ants[j].x][ants[j].y]]){
						case 1:
							ants[j].x += 1  ;
							ants[j].rule = 2;
							break;
						case 2:
							ants[j].x -= 1  ;
							ants[j].rule = 1;
							break;
						case 3:
							ants[j].y -= 1  ;
							ants[j].rule = 4;
							break;
						case 4:
							ants[j].y += 1  ;
							ants[j].rule = 3;
							break;
						}
					break;	
			}
			if (ants[j].x < 0 || ants[j].y < 0 || ants[j].x > width || ants[j].y > height){
				del_ant(ants, j, &n);
			}
			if (n != 0)
				j = (j + 1) % n;
			else
				flag = 0;
			
			if (j==0) iter++;
			if (iter == field.iteration) flag = 0;
			/* Каждый муравей будет ходить по порядку добавления
			(1 муравей может изменить цвет клетки на которой находится ещё не сходивший на этой итерации муравей)*/
			
			
            glPopMatrix ();
            Sleep(1);
            SwapBuffers (hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
    
}
