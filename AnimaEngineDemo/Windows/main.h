#ifndef __ANIMA_ENGINE_DEMO_H__
#define __ANIMA_ENGINE_DEMO_H__

bool InitEngine();

void SetViewport(int w, int h);
void UpdateFrame();

#if defined _WIN32
void RedirectIOToConsole();
#endif

#endif //__ANIMA_ENGINE_DEMO_H__