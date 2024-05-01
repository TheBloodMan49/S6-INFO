package tpspring.controller;

import lombok.NoArgsConstructor;
import org.apache.coyote.Response;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import tpspring.model.SpecificTodo;
import tpspring.model.Todo;
import tpspring.service.TodoServiceV2;

import java.security.Principal;
import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("api/v2/private/todo")
@CrossOrigin
@NoArgsConstructor
public class TodoControllerV2 {

    @Autowired
    private TodoServiceV2 service;

    @GetMapping(path = "todo/{id}", produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Todo> getTodoById(@PathVariable String id) {
        Todo todo = service.findTodo(Long.parseLong(id));
        if (todo == null) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND).body(null);
        }
        return ResponseEntity.ok(todo);
    }

    @PostMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Todo> newTodo(@RequestBody final Todo todo, final Principal principal) {
        todo.setOwner(principal.getName());
        return ResponseEntity.ok(service.addTodo(todo));
    }

    @DeleteMapping(path = "todo/{id}")
    public ResponseEntity<Void> deleteTodoById(@PathVariable String id, final Principal principal) {
        if (!principal.getName().equals(service.findTodo(Long.parseLong(id)).getOwner())) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN).build();
        }
        service.removeTodo(Long.parseLong(id));
        return ResponseEntity.ok().build();
    }

    @PutMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Void> updateTodoById(@RequestBody final Todo todo, final Principal principal) {
        if (!principal.getName().equals(todo.getOwner())) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN).build();
        }
        if (service.replaceTodo(todo)) {
            return ResponseEntity.ok().build();
        }
        return ResponseEntity.notFound().build();
    }

    @PatchMapping(path = "todo", consumes = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Void> patchTodoById(@RequestBody final Todo todo, final Principal principal) {
        if (!principal.getName().equals(todo.getOwner())) {
            return ResponseEntity.status(HttpStatus.FORBIDDEN).build();
        }
        Todo res = service.modifyTodo(todo);
        if (res == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok().build();
    }

    @GetMapping(path = "todo", produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<Iterable<Todo>> findByNameFragment(@RequestParam String name) {
        return ResponseEntity.ok(service.findByNameFragment(name));
    }

    @ExceptionHandler
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    public String send400(IllegalArgumentException e) {
        return e.getMessage();
    }

}
