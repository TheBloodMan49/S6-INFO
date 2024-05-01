package tpspring.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import tpspring.model.Todo;

import java.util.HashMap;
import java.util.Map;

@Service
public class TodoServiceV2 {

    @Autowired
    private TodoCRUDRepository repository;

    public TodoServiceV2() {
        //todos = new HashMap<>();
    }

    public Todo addTodo(Todo todo) {
        todo.setId(repository.count() + 1);
        return repository.save(todo);
    }

    public boolean replaceTodo(final Todo todo) {
        if (repository.existsById(todo.getId())) {
            repository.save(todo);
            return true;
        }
        return false;
    }

    public boolean removeTodo(final long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
            return true;
        }
        return false;
    }

    public Todo modifyTodo(final Todo partialTodo) {
        if (repository.findById(partialTodo.getId()).isPresent()) {
            Todo todo = repository.findById(partialTodo.getId()).get();
            if (partialTodo.getTitle() != null) {
                todo.setTitle(partialTodo.getTitle());
            }
            if (partialTodo.getDescription() != null) {
                todo.setDescription(partialTodo.getDescription());
            }
            if (partialTodo.getCategories() != null) {
                todo.setCategories(partialTodo.getCategories());
            }
            if (partialTodo.getList() != null) {
                todo.setList(partialTodo.getList());
            }
            if (partialTodo.getOwner() != null) {
                todo.setOwner(partialTodo.getOwner());
            }
            return repository.save(todo);
        }
        return null;
    }

    public Todo findTodo(final long id) {
        return repository.findById(id).orElse(null);
    }

    public Iterable<Todo> findByNameFragment(String nameFragment) {
        return repository.findByNameFragment(nameFragment);
    }
}
