
const float radius = 70;



int main()
{

	const float inferiorLimit = radius/2;
	const float xLimit = width - radius/2;
	const float yLimit = height - radius/2;

	float x = radius;
	float y = radius;
	float xMov = 0.02;
	float yMov = 0.02;

	float xMain = 400;
	float yMain = 300;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Inicio personagem principal

		if(leftPressed)
			if(xMain>inferiorLimit)
				xMain -= 0.08f;

		if(rightPressed)
			if(xMain<xLimit)
				xMain += 0.08f;

		if(topPressed)
			if(yMain<yLimit)
				yMain += 0.08f;

		if(downPressed)
			if(yMain>inferiorLimit)
				yMain -= 0.08f;

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xMain, yMain, 0));
		model = glm::rotate(model,glm::radians(180.0f), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(radius, radius, 1.0));
		shader.setMat4("model", glm::value_ptr(model));

		// Fim personagem principal

		glm::mat4 modelTwo = glm::mat4(1);
		modelTwo = glm::translate(modelTwo, glm::vec3(200, 200, 0));
		modelTwo = glm::rotate(modelTwo,glm::radians(180.0f), glm::vec3(0, 0, 1));
		modelTwo = glm::scale(modelTwo, glm::vec3(radius, radius, 1.0));
		shader.setMat4("modelTwo", glm::value_ptr(modelTwo));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return 0;
}

