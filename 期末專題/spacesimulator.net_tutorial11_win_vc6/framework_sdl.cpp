/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Framework functions (SDL)
 * SDL - Simple DirectMedia Layer - Sam Lantinga - www.libsdl.org
 *
 */

#ifdef FRAMEWORK_SDL

#include "sdl/include/sdl.h"
#include "framework.h"
#include "object.h"
#include "camera.h"
#include "render_ogl.h"
#include "malloc.h"
#include "load_ini.h"
#include "main.h"



/*
 *
 * VARIABLES DECLARATION
 *
 */

// Main window configuration
int framework_screen_width=640; // Width
int framework_screen_height=480; // Height
int framework_screen_depth; // Color depth
int framework_fullscreen=0; // Fullscreen

// Audio
SDL_AudioSpec framework_audiospec;
sound_type sounds[FRAMEWORK_MAXSOUNDS];



/*
 * int FrameworkInit()
 *
 * This function initializes the framework (rendering device, input and sound system)
 *
 * Return value: (char) 1 if the framework was initialized correctly otherwise it exit with ERRORLEVEL 0
 *
 */

int FrameworkInit(int *argcp,char *argv[])
{
	int l_video_flags=0;

	// Information about the current video settings
    const SDL_VideoInfo* info = NULL;

    fprintf( stdout, "SDL Framework initialization\n");

	// First, initialize SDL's video and audio subsystem
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) 
	{
        // Failed, exit
        fprintf( stdout, "Framework initialization failed: %s\n", SDL_GetError( ) );
        FrameworkQuit();
    }

    // Let's get some video information
    fprintf( stdout, "Getting video information\n");
    info = SDL_GetVideoInfo( );
    if( !info ) {
        // This should probably never happen
        fprintf( stdout, "Video query failed: %s\n", SDL_GetError( ) );
        FrameworkQuit();
    }

    /*
     * Set our width/height to 640/480 (you would
     * of course let the user decide this in a normal
     * app). We get the bpp we will request from
     * the display. On X11, VidMode can't change
     * resolution, so this is probably being overly
     * safe. Under Win32, ChangeDisplaySettings
     * can change the bpp.
     */
    framework_screen_depth = info->vfmt->BitsPerPixel;

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // Now we load the video settings from config.ini
	LoadINI("config.ini");
	framework_screen_width=LoadINI_GetInt("config.ini","screen settings","width",640);
	framework_screen_height=LoadINI_GetInt("config.ini","screen settings","height",480);
	framework_fullscreen=LoadINI_GetInt("config.ini","screen settings","fullscreen",0);
	l_video_flags = SDL_OPENGL;
	if (framework_fullscreen)
		l_video_flags|=SDL_FULLSCREEN;

    // Set the video mode
    fprintf( stdout, "Setting video mode\n");
    if( SDL_SetVideoMode( framework_screen_width, framework_screen_height, framework_screen_depth, l_video_flags ) == 0 ) {
        /* 
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         */
        fprintf( stdout, "Video mode set failed: %s\n", SDL_GetError( ) );
        FrameworkQuit();
    }

    // Keyboard repeat
    fprintf( stdout, "Enabling key repeat\n");
	if ( SDL_EnableKeyRepeat(10, SDL_DEFAULT_REPEAT_INTERVAL) ) // Key input delay of 10 ms and repeat every 100 msec	 
    {
        fprintf( stdout, "Failed enabling key repeat :", SDL_GetError() );
        FrameworkQuit();
    }  


    // Set the audio format
    framework_audiospec.freq = 22050;
    framework_audiospec.format = AUDIO_S16;
    framework_audiospec.channels = 2;    // 1 = mono, 2 = stereo
    framework_audiospec.samples = 1024;  // Good low-latency value for callback
    framework_audiospec.callback = FrameworkMixAudio;
    framework_audiospec.userdata = NULL;


    // Open the audio device, forcing the desired format
    fprintf( stdout, "Opening audio device\n");
    if ( SDL_OpenAudio(&framework_audiospec, NULL) < 0 ) {
        fprintf(stdout, "Couldn't open audio: %s\n", SDL_GetError());
        FrameworkQuit();
    }

	SDL_PauseAudio(0);

    fprintf( stdout, "Framework initialized correctly\n");
	return (1);
}



/*
 * void FrameworkQuit()
 *
 * Used to exit from the program
 *
 */

void FrameworkQuit()
{
    fprintf( stdout, "Exiting program\n");
	SDL_Quit();
	exit(0);
}



/*
 * void FrameworkMainLoop()
 *
 * Main loop, where our simulation runs!
 *
 */

void FrameworkMainLoop()
{
	fprintf( stdout, "Entering main loop\n");
	while( 1 ) 
	{
		MainLoop();
	}
}



/*
 * void resize(int p_width, int p_height)
 *
 * This routine must be called everytime we resize our window.
 *
 * Input parameters: p_width = width in pixels of our viewport
 *					 p_height = height in pixels of our viewport
 * 
 */

void FrameworkResize (int p_width, int p_height)
{
	//Resize currently unavailable
}



/*
 * void FrameworkEvents()
 *
 * Subroutine used to handle some events (keyboard, joystick inputs, framework quit)
 *
 */

void FrameworkEvents()
{
    SDL_Event event;
    while( SDL_PollEvent(& event) )                    
    {
		switch ( event.type )                          
        {
            case SDL_KEYDOWN :                                      
				KeyboardHandle(event.key.keysym.sym);
                break;
			case SDL_QUIT:
				FrameworkQuit();
				break;
			default:                                    
				break;                                  
		} 
	} 
}



/*
 * long Framework_GetTicks()
 *
 * Used to get the time in msec for time based animation and FPS calculation
 *
 */

long Framework_GetTicks(void)
{
	return (long)(SDL_GetTicks());
}



/*
 * void FrameworkSwapBuffers()
 *
 * Subroutine used to swap the color-buffers
 *
 * In double buffered mode the visible buffer is replaced with the writing buffer
 *
 */

void FrameworkSwapBuffers() 
{
	SDL_GL_SwapBuffers( );
}



/*
 * void FrameworkMixAudio(void *unused, Uint8 *stream, int len)
 *
 * To mix and play sounds, this is a callback function of SDL
 * For more information read the SDL documentation
 *
 */

void FrameworkMixAudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Uint32 l_amount;

    for ( i=0; i<FRAMEWORK_MAXSOUNDS; i++)
	{
        l_amount = (sounds[i].dlen-sounds[i].dpos);
        if ( l_amount > (Uint32)len ) 
		{
            l_amount = (Uint32)len;
		}
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], l_amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += l_amount;
    }
}



/*
 * void FrameworkAudioPlayWave(char *p_filename)
 *
 * To mix and play sounds
 *
 * Input parameters: p_filename = Filename of the sound to play
 *
 */

void FrameworkAudioPlayWave(char *file)
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    /* Look for an empty (or finished) sound slot */
    for ( index=0; index<FRAMEWORK_MAXSOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
    if ( index == FRAMEWORK_MAXSOUNDS )
        return;

    /* Load sound file and convert it to 16-bit stereo 22kHz */
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stdout, "Couldn't open %s: %s\n", 
                   file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16, 2, 22050);
    cvt.buf = (Uint8 *)malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);

    /* Insert the sound into the slot and play it */
    if ( sounds[index].data ) {
        free(sounds[index].data);
    }
    SDL_LockAudio();
    sounds[index].data = cvt.buf;
    sounds[index].dlen = cvt.len_cvt;
    sounds[index].dpos = 0;
    SDL_UnlockAudio();
}

#endif