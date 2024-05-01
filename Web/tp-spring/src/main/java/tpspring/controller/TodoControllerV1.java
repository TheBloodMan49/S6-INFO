package tpspring.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.*;
import tpspring.model.Todo;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("api/v1/public/todo")
@CrossOrigin
public class TodoControllerV1 {

    private final Map<Long, Todo> todos;
    private long nextId = 1;

    public TodoControllerV1() {
        todos = new HashMap<>();
        //todos.put(1L, new Todo(1L, "Faire les courses"));
    }

    @GetMapping(path = "todo/{id}", produces = MediaType.APPLICATION_JSON_VALUE)
    public Todo getTodoById(@PathVariable String id) {
        if (!todos.containsKey(Long.parseLong(id))) {
            throw new IllegalArgumentException("Todo not found");
        }
        return todos.get(Long.parseLong(id));
    }

    @PostMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
    public Todo newTodo(@RequestBody final Todo todo) {
        todo.setId(nextId++);
        todos.put(todo.getId(), todo);
        for (Todo t : todos.values()) {
            System.out.println(t);
        }
        return todo;
    }

    @DeleteMapping(path = "todo/{id}")
    public void deleteTodoById(@PathVariable String id) {
        if (todos.containsKey(Long.parseLong(id))) {
            todos.remove(Long.parseLong(id));
        } else {
            throw new IllegalArgumentException("Todo not found");
        }
    }

    @PutMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE)
    public void updateTodoById(@RequestBody final Todo todo) {
        if (todos.containsKey(todo.getId())) {
            todos.put(todo.getId(), todo);
        } else {
            throw new IllegalArgumentException("Todo not found");
        }
    }

    @PatchMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE)
    public void patchTodoById(@RequestBody final Todo todo) {
        if (todos.containsKey(todo.getId())) {
            if (todo.getTitle() != null) {
                todos.get(todo.getId()).setTitle(todo.getTitle());
            }
            if (todo.getDescription() != null) {
                todos.get(todo.getId()).setDescription(todo.getDescription());
            }
            if (todo.getCategories() != null) {
                todos.get(todo.getId()).setCategories(todo.getCategories());
            }
            if (todo.getList() != null) {
                todos.get(todo.getId()).setList(todo.getList());
            }
            if (todo.getOwner() != null) {
                todos.get(todo.getId()).setOwner(todo.getOwner());
            }
        } else {
            throw new IllegalArgumentException("Todo not found");
        }
    }

    @ExceptionHandler
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    public String send400(IllegalArgumentException e) {
        return e.getMessage();
    }

}
