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

struct LightProperties {
  GLfloat direction[3];
  GLfloat ambient_color[4];
  GLfloat diffuse_color[4];
  GLfloat specular_color[4];
};

struct MaterialProperties {
  GLfloat ambient_color[4];
  GLfloat diffuse_color[4];
  GLfloat specular_color[4];
  GLfloat specular_exponent;
};

struct LightLocations {
  GLint direction_location;
  GLint ambient_color_location;
  GLint diffuse_color_location;
  GLint specular_color_location;
};

struct MaterialLocations {
  GLint ambient_color_location;
  GLint diffuse_color_location;
  GLint specular_color_location;
  GLint specular_exponent_location;
};

static GLUSprogram g_program;

static GLint g_projection_matrix_location;

static GLint g_model_view_matrix_location;

static GLint g_normal_matrix_location;

static struct LightLocations g_light;

static struct MaterialLocations g_material;

static GLint g_vertex_location;

static GLint g_normal_location;

static GLuint g_vertices_VBO;

static GLuint g_normals_VBO;

static GLuint g_indices_VBO;

static GLuint g_vao;

static GLuint g_number_indices_sphere;

GLUSboolean init(GLUSvoid) {
  GLfloat view_matrix[16];
  GLfloat normal_matrix[9];

  struct LightProperties light = {
    { 1.0f, 1.0f, 1.0f },       // direction
    { 0.3f, 0.3f, 0.3f, 1.0f }, // ambient_c
    { 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse_c
    { 1.0f, 1.0f, 1.0f, 1.0f }  // specular_c
  };

  struct MaterialProperties material = {
    { 0.0f, 0.0f, 1.0f, 1.0f }, // ambient_c
    { 0.0f, 0.0f, 1.0f, 1.0f }, // diffuse_c
    { 1.0f, 1.0f, 1.0f, 1.0f }, // specular_c
    20.0f                       // specular_exponent
  };

  GLUStextfile vertex_source;
  GLUStextfile fragment_source;

  glusFileLoadText("/Users/xuyouren/XcodeProjects/GLUS/GLUS/phong.vert.glsl", &vertex_source);
  glusFileLoadText("/Users/xuyouren/XcodeProjects/GLUS/GLUS/phone.frag.glsl", &fragment_source);

  glusProgramBuildFromSource(&g_program, (const GLUSchar**) &vertex_source.text, 0, 0, 0, (const GLUSchar**) &fragment_source.text);

  glusFileDestroyText(&vertex_source);
  glusFileDestroyText(&fragment_source);


  g_projection_matrix_location = glGetUniformLocation(g_program.program, "u_projectionMatrix");

  g_model_view_matrix_location = glGetUniformLocation(g_program.program, "u_modelViewMatrix");

  g_normal_location = glGetUniformLocation(g_program.program, "u_normalMatrix");

  g_light.direction_location = glGetUniformLocation(g_program.program, "u_light.direction");
  g_light.ambient_color_location = glGetUniformLocation(g_program.program, "u_light.ambientColor");
  g_light.diffuse_color_location = glGetUniformLocation(g_program.program, "u_light.diffuseColor");
  g_light.specular_color_location = glGetUniformLocation(g_program.program, "u_light.specularColor");

  g_material.ambient_color_location = glGetUniformLocation(g_program.program, "u_material.ambientColor");
  g_material.diffuse_color_location = glGetUniformLocation(g_program.program, "u_material.diffuseColor");
  g_material.specular_color_location = glGetUniformLocation(g_program.program, "u_material.specularColor");
  g_material.specular_exponent_location = glGetUniformLocation(g_program.program, "u_material.specularExponent");

  g_vertex_location = glGetAttribLocation(g_program.program, "a_vertex");

  g_normal_location = glGetAttribLocation(g_program.program, "a_normal");

  GLUSshape sphere;
  glusShapeCreateSpheref(&sphere, 0.8f, 64);

  g_number_indices_sphere = sphere.numberIndices;

  glGenBuffers(1, &g_vertices_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertices_VBO);
  glBufferData(GL_ARRAY_BUFFER, sphere.numberVertices * 4 * sizeof(GLfloat), sphere.vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &g_normals_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, g_normals_VBO);
  glBufferData(GL_ARRAY_BUFFER, sphere.numberIndices * 3 * sizeof(GLfloat), sphere.normals, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &g_indices_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_VBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.numberIndices * sizeof(GLuint), sphere.indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glusShapeDestroyf(&sphere);

  glUseProgram(g_program.program);

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);

  glBindBuffer(GL_ARRAY_BUFFER, g_vertices_VBO);
  glVertexAttribPointer(g_vertex_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_vertex_location);

  glBindBuffer(GL_ARRAY_BUFFER, g_normals_VBO);
  glVertexAttribPointer(g_normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_normal_location);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_VBO);

  glusMatrix4x4LookAtf(
    view_matrix,
    0.0f, 0.0f, 5.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  );

  glusMatrix4x4ExtractMatrix3x3f(normal_matrix, view_matrix);

  glUniformMatrix4fv(g_model_view_matrix_location, 1, GL_FALSE, view_matrix);
  glUniformMatrix3fv(g_normal_matrix_location, 1, GL_FALSE, normal_matrix);

  glusVector3Normalizef(light.direction);

  glusMatrix4x4MultiplyVector3f(light.direction, view_matrix, light.direction);

  glUniform3fv(g_light.direction_location, 1, light.direction);
  glUniform4fv(g_light.ambient_color_location, 1, light.ambient_color);
  glUniform4fv(g_light.diffuse_color_location, 1, light.diffuse_color);
  glUniform4fv(g_light.specular_color_location, 1, light.specular_color);

  glUniform4fv(g_material.ambient_color_location, 1, material.ambient_color);
  glUniform4fv(g_material.diffuse_color_location, 1, material.diffuse_color);
  glUniform4fv(g_material.specular_color_location, 1, material.specular_color);
  glUniform1f(g_material.specular_exponent_location, material.specular_exponent);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST | GL_CULL_FACE);

  return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height) {
  GLfloat projection_matrix[16];

  glViewport(0, 0, width, height);

  glusMatrix4x4Perspectivef(projection_matrix, 40.0f, (GLfloat) width / height, 1.0f, 100.0f);

  glUniformMatrix4fv(g_projection_matrix_location, 1, GL_FALSE, projection_matrix);
}

GLUSboolean update(GLUSfloat time) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, g_number_indices_sphere, GL_UNSIGNED_INT, 0);
  return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if (g_vertices_VBO)
  {
    glDeleteBuffers(1, &g_vertices_VBO);

    g_vertices_VBO = 0;
  }

  if (g_normals_VBO)
  {
    glDeleteBuffers(1, &g_normals_VBO);

    g_normals_VBO = 0;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (g_indices_VBO)
  {
    glDeleteBuffers(1, &g_indices_VBO);

    g_indices_VBO = 0;
  }

  glBindVertexArray(0);

  if (g_vao)
  {
    glDeleteVertexArrays(1, &g_vao);

    g_vao = 0;
  }

  glUseProgram(0);

  glusProgramDestroy(&g_program);
}

int main (int argc, char ** argv) {
  EGLint eglConfigAttributes[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_DEPTH_SIZE, 24,
    EGL_STENCIL_SIZE, 0,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };

  EGLint eglContextAttributes[] = {
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

  if (!glusWindowCreate("GLUS Example 5", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes, 0)) {
    printf("could not create window!\n");
    exit(EXIT_FAILURE);
  }

  glusWindowRun();

  return EXIT_SUCCESS;
}