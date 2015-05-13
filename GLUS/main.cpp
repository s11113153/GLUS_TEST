//
//  main.cpp
//  GLUS
//
//  Created by 許佑任 on 2015/5/13.
//  Copyright (c) 2015年 許佑任. All rights reserved.
//

#include "GL/glus.h"

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

GLUSboolean init(GLUSvoid) {
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height) {
  glViewport(0, 0, width, height);
}

GLUSboolean update(GLUSfloat time) {
  glClear(GL_COLOR_BUFFER_BIT);
  return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid) {
  //do nothing
}

int main (int argc, char ** argv) {
  const GLUSchar *title = "GLUS_DEMO";
  const GLUSint width = 640;
  const GLUSint height = 480;
  const GLUSboolean fullscreen = GLUS_FALSE;
  const GLUSboolean noResize = GLUS_TRUE;

  EGLint egl_configAttribList[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_DEPTH_SIZE, 0,
    EGL_STENCIL_SIZE, 0,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };

  EGLint egl_contextAttribList[] = {
    EGL_CONTEXT_MAJOR_VERSION, 4,
    EGL_CONTEXT_MINOR_VERSION, 1,
    EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, EGL_TRUE,
    EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
    EGL_NONE
  };

  glusWindowSetInitFunc(init);

  glusWindowSetReshapeFunc(reshape);

  glusWindowSetUpdateFunc(update);

  glusWindowSetTerminateFunc(terminate);

  if (!glusWindowCreate(title, width, height, fullscreen, noResize, egl_configAttribList, egl_contextAttribList, 0)) {
    cout << "Could not create window" << endl;
    exit(EXIT_FAILURE);
  }

  glusWindowRun();

  return EXIT_SUCCESS;
}