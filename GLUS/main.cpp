#include <stdio.h>

#include "GL/glus.h"

static GLUSprogram g_program;

static GLint g_model_view_projection_matrix_location;

static GLint g_normal_matrix_location;

static GLint g_light_direction_location;

static GLint g_color_location;

static GLint g_vertex_location;

static GLint g_normal_location;

static GLuint g_vertices_VBO;

static GLuint g_normals_VBO;

static GLuint g_indices_VBO;

static GLuint g_vao;

static GLuint g_number_indices_cube;

GLUSboolean init(GLUSvoid) {
  GLfloat light_direction[3] = { 1.0f, 1.0f, 1.0f };
  GLfloat color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

  GLUStextfile vertex_source;
  GLUStextfile fragment_source;

  glusFileLoadText("/Users/xuyouren/XcodeProjects/GLUS/GLUS/basic.vert.glsl", &vertex_source);
  glusFileLoadText("/Users/xuyouren/XcodeProjects/GLUS/GLUS/color.frag.glsl", &fragment_source);

  glusProgramBuildFromSource(&g_program, (const GLUSchar **) &vertex_source.text, 0, 0, 0, (const GLUSchar **) &fragment_source.text);

  glusFileDestroyText(&vertex_source);
  glusFileDestroyText(&fragment_source);

  g_model_view_projection_matrix_location = glGetUniformLocation(g_program.program, "u_modelViewProjectionMatrix");
  g_normal_matrix_location = glGetUniformLocation(g_program.program, "u_normalMatrix");
  g_light_direction_location = glGetUniformLocation(g_program.program, "u_lightDirection");
  g_color_location = glGetUniformLocation(g_program.program, "u_color");

  g_vertex_location = glGetAttribLocation(g_program.program, "a_vertex");
  g_normal_location = glGetAttribLocation(g_program.program, "a_normal");

  GLUSshape cube;
  glusShapeCreateCubef(&cube, 0.8f);

  g_number_indices_cube = cube.numberIndices;

  glGenBuffers(1, &g_vertices_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertices_VBO);
  glBufferData(GL_ARRAY_BUFFER, cube.numberIndices * 4 * sizeof(GLfloat), cube.vertices, GL_STATIC_DRAW);


  glGenBuffers(1, &g_normals_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, g_normals_VBO);
  glBufferData(GL_ARRAY_BUFFER, cube.numberIndices * 3 * sizeof(GLfloat), cube.normals, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  glGenBuffers(1, &g_indices_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_VBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.numberIndices * sizeof(GLuint), cube.indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glusShapeDestroyf(&cube);

  glUseProgram(g_program.program);

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);

  glBindBuffer(GL_ARRAY_BUFFER, g_vertices_VBO);
  glVertexAttribPointer(g_vertex_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(g_vertex_location);

  glBindBuffer(GL_ARRAY_BUFFER, g_normals_VBO);
  glVertexAttribPointer(g_normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices_VBO);

  glusVector3Normalizef(light_direction);

  glUniform3fv(g_light_direction_location, 1, light_direction);

  glUniform4fv(g_color_location, 1, color);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);

  return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height) {
  GLfloat model_matrix[16];
  GLfloat norml_matrix[9];
  GLfloat view_matrix[16];
  GLfloat model_view_projection_matrix[16];

  glViewport(0, 0, width, height);

  glusMatrix4x4Identityf(model_matrix);

  glusMatrix4x4RotateRzRxRyf(model_matrix, 0.0f, 45.0f, 0.0f);

  glusMatrix4x4ExtractMatrix3x3f(norml_matrix, model_matrix);

  glusMatrix4x4Perspectivef(model_view_projection_matrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

  glusMatrix4x4LookAtf(view_matrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glusMatrix4x4Perspectivef(model_view_projection_matrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

  glusMatrix4x4LookAtf(view_matrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glusMatrix4x4Multiplyf(model_view_projection_matrix, model_view_projection_matrix, view_matrix);

  glusMatrix4x4Multiplyf(model_view_projection_matrix, model_view_projection_matrix, model_matrix);

  glUniformMatrix4fv(g_model_view_projection_matrix_location, 1, GL_FALSE, model_view_projection_matrix);

  glUniformMatrix3fv(g_normal_matrix_location, 1, GL_FALSE, norml_matrix);
}

GLUSboolean update(GLUSfloat time) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawElements(GL_TRIANGLES, g_number_indices_cube, GL_UNSIGNED_INT, 0);
  return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if (g_vertices_VBO) {
    glDeleteBuffers(1, &g_vertices_VBO);

    g_vertices_VBO = 0;
  }

  if (g_normals_VBO) {
    glDeleteBuffers(1, &g_normals_VBO);

    g_normals_VBO = 0;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if (g_indices_VBO) {
    glDeleteBuffers(1, &g_indices_VBO);

    g_indices_VBO = 0;
  }

  glBindVertexArray(0);

  if (g_vao) {
    glDeleteVertexArrays(1, &g_vao);

    g_vao = 0;
  }

  glUseProgram(0);

  glusProgramDestroy(&g_program);
}

int main(int argc, char* argv[]) {
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

  if (!glusWindowCreate("GLUS Example 4", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes, 0)) {
    printf("could not create window!\n");
    exit(EXIT_FAILURE);
  }

  glusWindowRun();

  return EXIT_SUCCESS;
}