package tpspring.service;

import org.springframework.stereotype.Service;
import tpspring.model.Todo;

import java.util.HashMap;
import java.util.Map;

@Service
public class TodoServiceV1 {

    private final Map<Long, Todo> todos;
    private long nextId = 1;
    public TodoServiceV1() {
        todos = new HashMap<>();
    }

    public Todo addTodo(final Todo todo) {
        todo.setId(nextId++);
        todos.put(todo.getId(), todo);
        return todo;
    }

    public boolean replaceTodo(final Todo todo) {
        if (todos.containsKey(todo.getId())) {
            todos.put(todo.getId(), todo);
            return true;
        }
        return false;
    }

    public boolean removeTodo(final long id) {
        if (todos.containsKey(id)) {
            todos.remove(id);
            return true;
        }
        return false;
    }

    public Todo modifyTodo(final Todo partialTodo) {
        if (todos.containsKey(partialTodo.getId())) {
            Todo todo = todos.get(partialTodo.getId());
            if (partialTodo.getTitle() != null) {
                todo.setTitle(todo.getTitle());
            }
            if (partialTodo.getDescription() != null) {
                todo.setDescription(todo.getDescription());
            }
            if (partialTodo.getCategories() != null) {
                todo.setCategories(todo.getCategories());
            }
            if (partialTodo.getList() != null) {
                todo.setList(todo.getList());
            }
            if (partialTodo.getOwner() != null) {
                todo.setOwner(todo.getOwner());
            }
            return todo;
        }
        return null;
    }

    public Todo findTodo(final long id) {
        return todos.get(id);
    }
}
