package tpspring.service;

import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.NoArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import tpspring.model.Todo;
import tpspring.model.TodoList;

import java.util.ArrayList;
import java.util.Map;
import java.util.Optional;

@Service
@NoArgsConstructor
public class TodoListService {
    @Autowired
    private TodoListCRUDRepository repository;

    @Autowired
    private TodoCRUDRepository todoRepository;

    @Autowired
    private ObjectMapper objectMapper;

    public TodoList createTodoList(TodoList todoList) {
        todoList.setTodos(new ArrayList<>());
        return repository.save(todoList);
    }

    public boolean replaceTodoList(final TodoList todoList) {
        if (repository.existsById(todoList.getId())) {
            repository.save(todoList);
            return true;
        }
        return false;
    }

    public boolean removeTodoList(final long id) {
        if (repository.existsById(id)) {
            repository.deleteById(id);
            return true;
        }
        return false;
    }

    public Optional<TodoList> patchTodoList(Map<String, Object> partialTodoList, long id) {
        Optional<TodoList> opt = repository.findById(id);
        if (opt.isEmpty()) {
            return Optional.empty();
        }
        TodoList todoList = opt.get();
        try {
            objectMapper.updateValue(todoList, partialTodoList);
        } catch (JsonMappingException e) {
            return Optional.empty();
        }
        return opt;
    }

    public Optional<TodoList> getTodoList(final long id) {
        return repository.findById(id);
    }

    public Optional<TodoList> addToTodoList(long listId, long todoId) {
        Optional<TodoList> list = repository.findById(listId);
        Optional<Todo> opt = todoRepository.findById(todoId);

        if (list.isPresent() && opt.isPresent()) {
            list.get().getTodos().add(opt.get());
            repository.save(list.get());
            opt.get().setList(list.get());
            todoRepository.save(opt.get());
            return list;
        }
        return Optional.empty();
    }
}
