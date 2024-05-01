package tpspring.service;

import java.util.List;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.context.junit.jupiter.SpringExtension;

import tpspring.model.Category;
import tpspring.model.Todo;
import tpspring.model.TodoList;
import tpspring.service.TodoCRUDRepository;

import static org.junit.jupiter.api.Assertions.*;

@ExtendWith(SpringExtension.class)
@SpringBootTest
public class TestTodoServiceV2 {
    // Mocking the repository
     @MockBean
     private TodoCRUDRepository repository;

     @Autowired
     private TodoServiceV2 todoService;

     Todo todo;
     Todo todo2;
     Todo todo3;

    @BeforeEach
    void setUp() {
        // To adapt
         todo = new Todo(1L, "title 1", "bar", List.of(Category.LOW_PRIORITY), null, "foo");
         todo2 = new Todo(2L, "title 2", "foo", List.of(Category.HIGH_PRIORITY), null, "you");
        todo3 = new Todo(3L, "title 3", "foo", List.of(Category.HIGH_PRIORITY), new TodoList(), "you");
    }

     @Test()
     public void saveCalledWhenAddingATodo() {
         // Configuring the mock so that a call to 'save' with 'todo' will return the same todo
         Mockito.when(repository.save(todo)).thenReturn(todo);

         Todo res = todoService.addTodo(todo);

         // Checking that the save method has been called one time with 'todo'
         Mockito.verify(repository, Mockito.times(1)).save(todo);
         // Checking that the method returns the result of 'save'.
         assertSame(todo, res);
     }

    @Test
    public void addTodo() {
        Mockito.when(repository.save(todo)).thenReturn(todo);

        Todo res = todoService.addTodo(todo);
        assertSame(todo, res);
    }

    @Test
    public void replaceTodo() {
        Mockito.when(repository.existsById(todo.getId())).thenReturn(true);
        Mockito.when(repository.save(todo)).thenReturn(todo);

        boolean res = todoService.replaceTodo(todo);
        assertTrue(res);
    }

    @Test
    public void removeTodo() {
        Mockito.when(repository.existsById(todo.getId())).thenReturn(true);

        boolean res = todoService.removeTodo(1L);
        assertTrue(res);
    }

    @Test
    public void modifyTodo() {
        Mockito.when(repository.findById(todo.getId())).thenReturn(java.util.Optional.of(todo));
        Mockito.when(repository.save(todo)).thenReturn(todo);

        Todo res = todoService.modifyTodo(todo);
        assertSame(todo, res);
    }

    @Test
    public void findTodo() {
        Mockito.when(repository.findById(todo.getId())).thenReturn(java.util.Optional.of(todo));

        Todo res = todoService.findTodo(1L);
        assertSame(todo, res);
    }

    @Test
    public void findTodoNotFound() {
        Mockito.when(repository.findById(todo.getId())).thenReturn(java.util.Optional.empty());

        Todo res = todoService.findTodo(1L);
        assertNull(res);
    }

    @Test
    public void findByNameFragment() {
        Mockito.when(repository.findByNameFragment("t")).thenReturn(List.of(todo, todo2));

        List<Todo> res = (List<Todo>) todoService.findByNameFragment("t");
        assertEquals(2, res.size());
        assertSame(todo, res.get(0));
        assertSame(todo2, res.get(1));
    }

    @Test
    public void findByNameFragmentEmpty() {
        Mockito.when(repository.findByNameFragment("t")).thenReturn(List.of());

        List<Todo> res = (List<Todo>) todoService.findByNameFragment("t");
        assertEquals(0, res.size());
    }

    @Test
    public void replaceNonExistingTodo() {
        Mockito.when(repository.existsById(todo.getId())).thenReturn(false);

        boolean res = todoService.replaceTodo(todo);
        assertFalse(res);
    }

    @Test
    public void removeNonExistingTodo() {
        Mockito.when(repository.existsById(todo.getId())).thenReturn(false);

        boolean res = todoService.removeTodo(1L);
        assertFalse(res);
    }

    @Test
    public void modifyNonExistingTodo() {
        Mockito.when(repository.findById(todo.getId())).thenReturn(java.util.Optional.empty());

        Todo res = todoService.modifyTodo(todo);
        assertNull(res);
    }

    @Test
    public void modifyFullTodo() {
        Mockito.when(repository.findById(todo3.getId())).thenReturn(java.util.Optional.of(todo3));
        Mockito.when(repository.save(todo3)).thenReturn(todo3);

        Todo res = todoService.modifyTodo(todo3);
        assertSame(todo3, res);
    }
}
