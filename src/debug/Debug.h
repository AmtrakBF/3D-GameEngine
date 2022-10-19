#pragma once
#include "glm/glm.hpp"
#include <iostream>

#define PRINT(value) Output((value), #value)
#define GET_VARIABLE_NAME(var) (#var)

class Debug
{
private:
	Debug() {}
	~Debug() {}
	Debug(const Debug& debug) {}
	Debug& operator = (const Debug& debug) {}


public:
	static Debug* Instance();

	//! probably a better way to do this rather than just copy and pasting the same code over and over but it works

	//! Call PRINT #definition to automatically place variable name as title
	inline void Output(glm::vec3 input, const char* title = "") const { std::cout << title << ": " << input.x << ", " << input.y << ", " << input.z << std::endl; };
	inline void Output(glm::vec3 (*func)(), const char* title = "") const { std::cout << title << ": " << func().x << ", " << func().y << ", " << func().z << std::endl; };

	inline void Output(glm::vec2 input, const char* title = "") const { std::cout << title << ": " << input.x << ", " << input.y << std::endl; };
	inline void Output(glm::vec2(*func)(), const char* title = "") const { std::cout << title << ": " << func().x << ", " << func().y << std::endl; };

	inline void Output(float input, const char* title = "") const { std::cout << title << ": " << input << std::endl; };
	inline void Output(float (*func)(), const char* title = "") const { std::cout << title << ": " << func() << std::endl; };

	inline void Output(int input, const char* title = "") const { std::cout << title << ": " << input << std::endl; };
	inline void Output(int (*func)(), const char* title = "") const { std::cout << title << ": " << func() << std::endl; };
	
	inline void Output(uint32_t input, const char* title = "") const { std::cout << title << ": " << input << std::endl; };
	inline void Output(uint32_t(*func)(), const char* title = "") const { std::cout << title << ": " << func() << std::endl; };

	inline void Output(const char* input, const char* title = "") const { std::cout << title << ": " << input << std::endl;  };
	inline void Output(const char* (*func)(), const char* title = "") const { std::cout << title << ": " << func() << std::endl; };
};