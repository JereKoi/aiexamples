#include <stdint.h>
#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <memory.h>
#include <glad/gl.h>		// Include glad
#include <GLFW/glfw3.h>
#include <vector>
#include "agent.h"




namespace
{
	uint8_t* getPixel(int x, int y, uint8_t* data, int width) {
		return &data[3 * (y * width + x)];
	}

	const uint8_t* getPixel(int x, int y, const uint8_t* data, int width) {
		return &data[3 * (y * width + x)];
	}

	void setPixel(int x, int y, uint8_t* data, int width, uint8_t r, uint8_t g, uint8_t b) {
		auto pixel = getPixel(x, y, data, width);
		pixel[0] = b;
		pixel[1] = g;
		pixel[2] = r;
	}

	bool isGreenPixel(core::Vector state, const uint8_t* data, int width) {
		auto pixel = getPixel(state[0], state[1], data, width);
		// pixel is green if r and b are small and g is big
		return pixel[0] < 20 &&  pixel[1] > 200 && pixel[2] < 20;  
	}

	// STUDENT_TODO: Make implementation for doPathFinding function, which writes found path to outputData
	void doPathFinding(const uint8_t* inputData, int width, int height, uint8_t* outputData, int startX, int startY, int endX, int endY)
	{
		printf("STUDENT_TODO: Do path finding from <%d,%d> to <%d,%d>\n", startX, startY, endX, endY);
		
		std::vector<int> startState = {startX, startY};
		std::vector<int> goalState = { endX, endY };

		ActionsVector actions;
		actions.push_back([](std::vector<int> s) -> std::vector<int> {
			s[0] += 1; // Step right
			return s;
		});
		actions.push_back([](std::vector<int> s) -> std::vector<int> {
			s[0] -= 1; // Step left
			return s;
		});
		actions.push_back([](std::vector<int> s) -> std::vector<int> {
			s[1] += 1; // Step down
			return s;
		});
		actions.push_back([](std::vector<int> s) -> std::vector<int> {
			s[1] -= 1; // Step up
			return s;
		});

		// Make noise for now
		/*for (size_t i = 0; i < 3 * width * height; i += 3)
		{
			int val = rand();
			outputData[i+0] = 0; // Blue
			outputData[i+1] = 0; // Green
			outputData[i+2] = 0; // Red
		}*/
#if 0
		// Set screen black and put some pixels.
		memset(outputData, 0, 3*width*height);
		setPixel(0, 0, outputData, width, 0xff, 0x00, 0x00);
		setPixel(1, 0, outputData, width, 0xff, 0x00, 0xff);
		setPixel(2, 0, outputData, width, 0xff, 0xff, 0x00);
		setPixel(3, 0, outputData, width, 0x00, 0xff, 0xff);
		setPixel(4, 0, outputData, width, 0x00, 0x00, 0xff);
#endif
		memcpy(outputData, inputData, 3*width*height); // Copy the input to putput

		//auto openList = aStar(startState, goalState, inputData, width, height, actions);

		// TODO: Find a path from beginnig to end and color it to screen using "magenta" (blue+red) pixels.
		//setPixel(4, 4, outputData, width, 0xff, 0x00, 0xff);
	}
}


namespace
{
	// Quick and dirty function for reading bmp-files to opengl textures.
	GLuint loadBMPTexture(const char *fileName, int* w, int* h, uint8_t** data)
	{
		assert(w != 0);
		assert(h != 0);
		assert(data != 0);
		FILE *file;
		if ((file = fopen(fileName, "rb")) == NULL)
			return 0;
		fseek(file, 18, SEEK_SET);

		int width = 0;
		fread(&width, 2, 1, file);
		fseek(file, 2, SEEK_CUR);
		int height = 0;
		fread(&height, 2, 1, file);
		printf("Image \"%s\" (%dx%d)\n", fileName, width, height);

		*data = new uint8_t[3 * width * height];
		assert(data != 0);
		fseek(file, 30, SEEK_CUR);
		fread(*data, 3, width * height, file);
		fclose(file);

		GLuint  texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, *data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, *data);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (w) *w = width;
		if (h) *h = height;
		return texId;
	}

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Global variables

	// OpenGL texture ids for rendering.
	GLuint  inputTexture = 0;
	GLuint  outputTexture = 0;
	// Input and output data in pixels. outputData is updated to outputTexture each frame
	uint8_t* inputData = 0;
	uint8_t* outputData = 0;
	// width and height of the input and output datas
	int width = 0;
	int height = 0;
	// start and end position for path finding. These are found automatically from input file.
	int startX = -1;
	int startY = -1;
	int endX = -1;
	int endY = -1;

	// Initialization
	bool init()
	{
		glMatrixMode(GL_PROJECTION);
		glOrtho(0, 512 + 4, 256 + 2, 0, -1, 1);

		// Load input file
		inputTexture = loadBMPTexture("input.bmp", &width, &height, &inputData);
		if (0 == inputTexture)
		{
			printf("Error! Cannot open file: \"input.bmp\". Please copy it to build-folder.\n");
			return false;
		}

		// Make outputTexture
		glGenTextures(1, &outputTexture);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Copy inputData also to outputData
		outputData = new uint8_t[3 * width*height];
		memcpy(outputData, inputData, 3 * width*height);

		// find start and end
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				uint8_t* pix = &inputData[3 * (y*width + x)]; // get pixel
				uint8_t r = pix[0];
				uint8_t g = pix[1];
				uint8_t b = pix[2];
				if (255 == r && g == 0 && b == 0) // Red?
				{
					// Start
					startX = x;
					startY = y;
					printf("Start position: <%d,%d>\n", x, y);
				}
				if (255 == b && r == 0 && g == 0) // Blue?
				{
					// End
					endX = x;
					endY = y;
					printf("End position: <%d,%d>\n", x, y);
				}
			}
		}

		if (startX < 0 || startY < 0)
		{
			printf("Error! Start position not found\n");
			return false;
		}

		if (endX < 0 || endY < 0)
		{
			printf("Error! End position not found\n");
			return false;
		}

		return true;
	}

	// Draw/Render
	bool draw(GLFWwindow* window)
	{
		doPathFinding(inputData, width, height, outputData, startX, startY, endX, endY);

		// Copy outputData to outputTexture
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, outputData);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, outputData);
		glBindTexture(GL_TEXTURE_2D, 0);

		glClear(GL_COLOR_BUFFER_BIT);

		// Draw input texture to left half of the screen
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, inputTexture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(1, 1);
		glTexCoord2d(0, 0); glVertex2d(1, 1 + 256);
		glTexCoord2d(1, 0); glVertex2d(1 + 256, 1 + 256);
		glTexCoord2d(1, 1); glVertex2d(1 + 256, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Draw output texture to right half of the screen
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, outputTexture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(2 + 256, 1);
		glTexCoord2d(0, 0); glVertex2d(2 + 256, 1 + 256);
		glTexCoord2d(1, 0); glVertex2d(2 + 512, 1 + 256);
		glTexCoord2d(1, 1); glVertex2d(2 + 512, 1);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glfwSwapBuffers(window);
		// Poll other window events.
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			return false;
		}
		return true;
	}
} // end - anonymous namespace

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// Main
int main(int argc, char ** argv) {
	if (!glfwInit()) {
		// Initialization failed
		return -1;
	}
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(2 * (512 + 4), 2 * (256 + 2), "Path find exercise", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	if (!init()) {
		return -1;
	}

	while (draw(window)) {

	}

	glfwDestroyWindow(window);

	delete inputData;
	delete outputData;

	glfwTerminate();
	return 0;
}