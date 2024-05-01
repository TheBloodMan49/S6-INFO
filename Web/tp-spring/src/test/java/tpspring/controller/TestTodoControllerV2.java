package tpspring.controller;

import static org.hamcrest.collection.IsCollectionWithSize.hasSize;
import static org.hamcrest.core.IsEqual.equalTo;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import java.util.List;

import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.annotation.DirtiesContext;
import org.springframework.test.web.servlet.MockMvc;

import tpspring.model.Category;
import tpspring.model.Todo;
import tpspring.service.TodoServiceV2;

@SpringBootTest
@AutoConfigureMockMvc
@DirtiesContext(classMode = DirtiesContext.ClassMode.AFTER_EACH_TEST_METHOD)
public class TestTodoControllerV2 {
	@Autowired
	private MockMvc mvc;

	 @MockBean
     private TodoServiceV2 todoService;

	@Test
	void testHello() throws Exception {
		mvc.perform(get("/api/v1/public/hello/helloworld"))
			.andExpect(status().isOk())
			.andExpect(content().contentTypeCompatibleWith(MediaType.TEXT_PLAIN))
			.andExpect(content().string(equalTo("Hello World")));
	}

	 @Test
	 void getTodo() throws Exception {
	 	Mockito.when(todoService.findTodo(1L))
	 		.thenReturn(new Todo(1L, "t1", "desc", List.of(Category.LOW_PRIORITY), null, "foo"));

	 	mvc.perform(get("/api/v2/public/todo/todo/1"))
	 			.andExpect(status().isOk())
	 			.andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
	 			.andExpect(jsonPath("$.title", equalTo("t1")))
	 			.andExpect(jsonPath("$.description", equalTo("desc")))
	 			.andExpect(jsonPath("$.categories", hasSize(1)))
	 			.andExpect(jsonPath("$.categories[0]", equalTo("LOW_PRIORITY")))
	 			.andExpect(jsonPath("$.owner", equalTo("foo")))
	 			.andExpect(jsonPath("$.id", equalTo(1)))
	 			.andExpect(jsonPath("$.*", hasSize(6)));
	 }

	 @Test
	 void getTodoNotFound() throws Exception {
	 	Mockito.when(todoService.findTodo(1L))
	 		.thenReturn(null);

	 	mvc.perform(get("/api/v2/public/todo/todo/1"))
	 			.andExpect(status().isNotFound());
	 }

	 @Test
	 void getTodoByNameFragment() throws Exception {
	 	Mockito.when(todoService.findByNameFragment("t"))
	 		.thenReturn(List.of(new Todo(1L, "t1", "desc", List.of(Category.LOW_PRIORITY), null, "foo"),
	 				new Todo(2L, "t2", "desc", List.of(Category.LOW_PRIORITY), null, "foo")));

	 	mvc.perform(get("/api/v2/public/todo/todo?name=t"))
	 			.andExpect(status().isOk())
	 			.andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
	 			.andExpect(jsonPath("$", hasSize(2)))
	 			.andExpect(jsonPath("$[0].title", equalTo("t1")))
	 			.andExpect(jsonPath("$[1].title", equalTo("t2")));
	 }

	 @Test
	 void getTodoByNameFragmentEmpty() throws Exception {
	 	Mockito.when(todoService.findByNameFragment("t"))
	 		.thenReturn(List.of());

	 	mvc.perform(get("/api/v2/public/todo/todo?name=t"))
	 			.andExpect(status().isOk())
	 			.andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
	 			.andExpect(jsonPath("$", hasSize(0)));
	 }

	 @Test
	 void newTodo() throws Exception {
	 	Mockito.when(todoService.addTodo(new Todo(0L, "t1", "desc", List.of(Category.LOW_PRIORITY), null, "foo")))
	 		.thenReturn(new Todo(1L, "t1", "desc", List.of(Category.LOW_PRIORITY), null, "foo"));

	 	mvc.perform(post("/api/v2/public/todo/todo")
				.contentType(MediaType.APPLICATION_JSON_VALUE)
	 			.content("{\"title\":\"t1\",\"description\":\"desc\",\"categories\":[\"LOW_PRIORITY\"],\"owner\":\"foo\"}"))
				.andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON_VALUE))
	 			.andExpect(status().isOk())
	 			.andExpect(jsonPath("$.title", equalTo("t1")))
	 			.andExpect(jsonPath("$.description", equalTo("desc")))
	 			.andExpect(jsonPath("$.categories", hasSize(1)))
	 			.andExpect(jsonPath("$.categories[0]", equalTo("LOW_PRIORITY")))
	 			.andExpect(jsonPath("$.owner", equalTo("foo")))
	 			.andExpect(jsonPath("$.id", equalTo(1)))
	 			.andExpect(jsonPath("$.*", hasSize(6)));
	 }

	 @Test
	 void updateTodo() throws Exception {
	 	Mockito.when(todoService.replaceTodo(new Todo(1L, "t1", "desc", List.of(Category.LOW_PRIORITY), null, "foo")))
	 		.thenReturn(true);

	 	mvc.perform(get("/api/v2/public/todo/todo/1"))
	 			.andExpect(status().isOk())
	 			.andExpect(content().contentTypeCompatibleWith(MediaType.APPLICATION_JSON))
	 			.andExpect(jsonPath("$.title", equalTo("t1")))
	 			.andExpect(jsonPath("$.description", equalTo("desc")))
	 			.andExpect(jsonPath("$.categories", hasSize(1)))
	 			.andExpect(jsonPath("$.categories[0]", equalTo("LOW_PRIORITY")))
	 			.andExpect(jsonPath("$.owner", equalTo("foo")))
	 			.andExpect(jsonPath("$.id", equalTo(1)))
	 			.andExpect(jsonPath("$.*", hasSize(6)));
	 }
}
