package tpspring.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import tpspring.controller.dto.NamedDTO;
import tpspring.model.Todo;
import tpspring.model.TodoList;
import tpspring.service.TodoListService;

import java.util.List;
import java.util.Map;
import java.util.Optional;

@RestController
@CrossOrigin
@RequestMapping("api/v2/private/todolist")
public class TodoListController {
    @Autowired
    private TodoListService service;

    public TodoListController() {
    }

    @GetMapping(path = "todolist/{id}", produces = "application/json")
    public ResponseEntity<TodoList> getTodoListById(@PathVariable String id) {
        Optional<TodoList> opt = service.getTodoList(Long.parseLong(id));
        return opt.map(ResponseEntity::ok).orElseGet(() -> ResponseEntity.notFound().build());
    }

    @PostMapping(path = "todolist", consumes = "application/json", produces = "application/json")
    public ResponseEntity<TodoList> newTodoList(@RequestBody final NamedDTO todoList) {
        return ResponseEntity.ok(service.createTodoList(todoList.getTodoList()));
    }

    @PostMapping(path = "todolist/{listId}/{todoId}", produces = "application/json")
    public ResponseEntity<TodoList> addToTodoList(@PathVariable String listId, @PathVariable String todoId) {
        Optional<TodoList> opt = service.addToTodoList(Long.parseLong(listId), Long.parseLong(todoId));
        return opt.map(ResponseEntity::ok).orElseGet(() -> ResponseEntity.notFound().build());
    }

    @DeleteMapping(path = "todolist/{id}")
    public ResponseEntity<Void> deleteTodoListById(@PathVariable String id) {
        if (service.removeTodoList(Long.parseLong(id))) {
            return ResponseEntity.noContent().build();
        }
        return ResponseEntity.notFound().build();
    }

    @PutMapping(path = "todolist", consumes = "application/json")
    public ResponseEntity<Void> updateTodoListById(@RequestBody final TodoList todoList) {
        if (service.replaceTodoList(todoList)) {
            return ResponseEntity.noContent().build();
        }
        return ResponseEntity.notFound().build();
    }

    @PatchMapping(path = "todolist/{id}", consumes = "application/json", produces = "application/json")
    public ResponseEntity<TodoList> patchTodoListById(@PathVariable String id, @RequestBody Map<String, Object> partialTodoList) {
        Optional<TodoList> opt = service.patchTodoList(partialTodoList, Long.parseLong(id));
        return opt.map(ResponseEntity::ok).orElseGet(() -> ResponseEntity.notFound().build());
    }

}
