#include "config.h"


static std::string readFile(std::string fileName){
    std::stringstream fileBuffer;
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cerr << "File Not Open" << std::endl;
        return nullptr;
    }
    fileBuffer << file.rdbuf();
    file.close();
    return fileBuffer.str();
} 

static unsigned int CompileShader(unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
        << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main() {
    GLFWwindow* window;

    if (!glfwInit()){
        std::cout << "GLFW couldn't start" << std::endl;
        return -1;
    }
    
    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    
    glfwMakeContextCurrent(window);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    

    float position[] = {
        -.5f, -.5f,
        0.0f, .5f,
        .5f, -.5f
    };
    
    for(int i = 0; i < 6; i++){
        std::cout << position[i] << std::endl;
    }

    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2 , 0);
    glEnableVertexAttribArray(0);
    

    glClearColor(1.0f,.0f,.5f,1.0f);

    unsigned int shader = CreateShader(readFile("../src/basic.vert"), readFile("../src/basic.frag"));
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
    
}