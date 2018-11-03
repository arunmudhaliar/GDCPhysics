#include "appEntry.h"
#include <iostream>

#include "Physics/scene.h"

bool g_mControlKeyPressed = false;
bool g_mAppInitialised = false;
bool g_mReInitMono = false;
//int handleAppEvents(void *userdata, SDL_Event *event);

int GDCPhysics::appEntry() {
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("ERROR SDL_Init\n");
        return -1;
    }

//    SDL_SetEventFilter(handleAppEvents, NULL);

    // create a window
    SDL_Window * window = SDL_CreateWindow(
                                           "GDCPhysics",             // window title
                                           SDL_WINDOWPOS_CENTERED,     // x position, centered
                                           SDL_WINDOWPOS_CENTERED,     // y position, centered
                                           800,                        // width, in pixels
                                           600,                        // height, in pixels
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  /*| SDL_WINDOW_RESIZABLE  |  SDL_WINDOW_ALLOW_HIGHDPI */   // flags
                                           );
	
    // SDL_MaximizeWindow(window);

    int window_cx=1;
    int window_cy=1;
    SDL_GetWindowSize(window, &window_cx, &window_cy);
    SDL_GL_GetDrawableSize(window, &window_cx, &window_cy);
    
    if( window == NULL ) {
        std::cout << "ERROR SDL_CreateWindow" << std::endl;
        return -1;
    }

    //shared context
    //http://forums.libsdl.org/viewtopic.php?t=9036&sid=3374c819e18df779e17b4ce5a49fdd15
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    //Create context
    SDL_GLContext context = SDL_GL_CreateContext( window );
    if( context == NULL ) {
        std::cout << "ERROR SDL_GL_CreateContext" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Scene gameScene;
    gameScene.InitScene(window_cx, window_cy);
    
    SDL_GL_MakeCurrent(window, context);
    bool quit=false;
    //While application is running
    while( !quit ) {
        SDL_Event e;
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
            } else {
                Uint32 event_wndID=e.window.windowID;
                processSDLEvent(SDL_GetWindowFromID(event_wndID), e, &gameScene);
            }
        }

        //rendering loop
        gameScene.Update();
        gameScene.Render();
        
        SDL_Delay(rand()%10);
        
        //swapbuffer
        SDL_GL_SwapWindow( window );
    }
    
    //clean up
    SDL_GL_DeleteContext(context);  //DeleteContext will be called by renderer. So no need to call it here.
    SDL_DestroyWindow(window);
    SDL_Quit();
        
    return 0;
}

/*
int handleAppEvents(void *userdata, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_APP_TERMINATING:
//            Terminate the app.
//            Shut everything down before returning from this function.
            printf("SDL_APP_TERMINATING");
            return 0;
        case SDL_APP_LOWMEMORY:
//            You will get this when your app is paused and iOS wants more memory.
//            Release as much memory as possible.
            printf("SDL_APP_LOWMEMORY");
            return 0;
        case SDL_APP_WILLENTERBACKGROUND:
//            Prepare your app to go into the background.  Stop loops, etc.
//            This gets called when the user hits the home button, or gets a call.
            printf("SDL_APP_WILLENTERBACKGROUND");
            return 0;
        case SDL_APP_DIDENTERBACKGROUND:
//            This will get called if the user accepted whatever sent your app to the background.
//            If the user got a phone call and canceled it, you'll instead get an    SDL_APP_DIDENTERFOREGROUND event and restart your loops.
//            When you get this, you have 5 seconds to save all your state or the app will be terminated.
//            Your app is NOT active at this point.
            printf("SDL_APP_DIDENTERBACKGROUND");
            return 0;
        case SDL_APP_WILLENTERFOREGROUND:
//            This call happens when your app is coming back to the foreground.
//            Restore all your state here.
            printf("SDL_APP_WILLENTERFOREGROUND");
            return 0;
        case SDL_APP_DIDENTERFOREGROUND:
//            Restart your loops here.
//            Your app is interactive and getting CPU again.
            printf("SDL_APP_DIDENTERFOREGROUND");
            return 0;
        default:
//            No special processing, add it to the event queue
            return 1;
    }
}
*/

void GDCPhysics::processSDLEvent(SDL_Window * window, SDL_Event& e, void* userData) {
    Scene* scene = static_cast<Scene*>(userData);
    
    if(e.type==SDL_WINDOWEVENT) {
        SDL_WindowEvent* windowEvent = (SDL_WindowEvent*)&e;

        switch (windowEvent->event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                int window_cx=1;
                int window_cy=1;
                SDL_GetWindowSize(window, &window_cx, &window_cy);
                SDL_GL_GetDrawableSize(window, &window_cx, &window_cy);
                scene->Resize(window_cx, window_cy);
            }
                break;
            
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                printf("SDL_WINDOWEVENT_FOCUS_LOST\n");
                if(g_mAppInitialised)
                {
//                    if(!monoWrapper::mono_isSimulationRunning())
//                    {
//                        EditorGEARApp::saveSceneToTempFolder();
//                    }
                }
            }
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");

                if(g_mAppInitialised)
                {
                    g_mReInitMono = true;
                    printf("ReInit mono after one loop\n");
                }
                else
                {
                    g_mAppInitialised = true;
                }
            }
                break;
                
            case SDL_WINDOWEVENT_SHOWN:
            {
                printf("SDL_WINDOWEVENT_SHOWN\n");
            }
                break;

            case SDL_WINDOWEVENT_EXPOSED:
            {
                printf("SDL_WINDOWEVENT_EXPOSED\n");
            }
                break;

            default:
                break;
        }
    }
    else if(e.type==SDL_KEYDOWN)
    {
        SDL_KeyboardEvent* keyBoardEvent = (SDL_KeyboardEvent*)&e;
        //const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		g_mControlKeyPressed = (keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL);
//        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
//        {
//            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyDown(keyBoardEvent->keysym.scancode, keyBoardEvent->keysym.mod);
//            UNUSED(bCaptured);
//        }
//        else
//        {
//            editorApp.KeyDown(keyBoardEvent->keysym.scancode, 0/*lParam*/);
//        }
    }
    else if(e.type==SDL_KEYUP)
    {
        SDL_KeyboardEvent* keyBoardEvent = (SDL_KeyboardEvent*)&e;
        //const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if(g_mControlKeyPressed)
        {
            if((keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL))
                g_mControlKeyPressed = false;
        }

//        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
//        {
//            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyUp(keyBoardEvent->keysym.scancode, keyBoardEvent->keysym.mod);
//            UNUSED(bCaptured);
//        }
//        else
//        {
//            editorApp.KeyUp(keyBoardEvent->keysym.scancode, 0/*lParam*/);
//        }
    }
    else if(e.type==SDL_MOUSEBUTTONDOWN)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        SDL_MouseButtonEvent* mouseBtnEvent = (SDL_MouseButtonEvent*)&e;
        switch (mouseBtnEvent->button) {
            case SDL_BUTTON_LEFT:
            {
//                //DEBUG_PRINT("Left Mouse Down");
//                geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
//                editorApp.MouseLButtonDown(mouse_x, mouse_y, MK_LBUTTON | ((g_mControlKeyPressed)?MK_CONTROL:0));
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
//                //DEBUG_PRINT("Middle Mouse Down");
//                editorApp.MouseMButtonDown(mouse_x, mouse_y, MK_MBUTTON);
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
//                //DEBUG_PRINT("Right Mouse Down");
//                editorApp.MouseRButtonDown(mouse_x, mouse_y, MK_RBUTTON);
            }
                break;
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEBUTTONUP)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        SDL_MouseButtonEvent* mouseBtnEvent = (SDL_MouseButtonEvent*)&e;
        switch (mouseBtnEvent->button) {
            case SDL_BUTTON_LEFT:
            {
//                //DEBUG_PRINT("Left Mouse Up");
//                //geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
//                editorApp.MouseLButtonUp(mouse_x, mouse_y, MK_LBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                scene->MouseBtnUp();
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
//                //DEBUG_PRINT("Middle Mouse Up");
//                editorApp.MouseMButtonUp(mouse_x, mouse_y, MK_MBUTTON);
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
//                //DEBUG_PRINT("Right Mouse Up");
//                editorApp.MouseRButtonUp(mouse_x, mouse_y, MK_RBUTTON);
            }
                break;
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEMOTION)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        //DEBUG_PRINT("m_x=%d, m_y%d", mouse_x, mouse_y);
        SDL_MouseMotionEvent* mouseMotionEvent = (SDL_MouseMotionEvent*)&e;
        switch (mouseMotionEvent->state) {
            case 1:
//                editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_LBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;
            case 2:
//                editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_MBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;
            case 4:
//                editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_RBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;

            default:
//                editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, 0);
                break;
        }
    }
    else if(e.type==SDL_MOUSEWHEEL)
    {
//        int mouse_x = 0, mouse_y = 0;
//        SDL_GetMouseState( &mouse_x, &mouse_y );
//
//        int nFlags = (g_mControlKeyPressed) ? MK_CONTROL : 0;
//        SDL_MouseWheelEvent* mouseWheelEvent = (SDL_MouseWheelEvent*)&e;
//        editorApp.MouseWheel(mouseWheelEvent->y, mouse_x, mouse_y, nFlags);
    }
    else if (e.type==SDL_DROPFILE)
    {
//        int mouse_x = 0, mouse_y = 0;
//        SDL_GetMouseState( &mouse_x, &mouse_y );
//        SDL_DropEvent* dropEvent = (SDL_DropEvent*)&e;
//        MDropData* dropData = (MDropData*)dropEvent->file;
//        editorApp.DragDrop(mouse_x, mouse_y, dropData);
//        GE_DELETE(dropData);
//        //SDL_FlushEvent(SDL_MOUSEMOTION);  //FlushEvent is called inside geGUIBase.
    }
}
