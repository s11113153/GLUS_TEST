#version 410
uniform mat4 u_modelViewProjectionMatrix;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightDirection;
uniform vec4 u_color;

in vec4 a_vertex;
in vec3 a_normal;

out vec4 v_color;

void main(void) {
  // normal in world space, we can pass the light in world space
  vec3 normal = u_normalMatrix * a_normal;

 	v_color = u_color * 0.3 + u_color * max(dot(normal, u_lightDirection), 0.0);

  gl_Position = u_modelViewProjectionMatrix * a_vertex;
}