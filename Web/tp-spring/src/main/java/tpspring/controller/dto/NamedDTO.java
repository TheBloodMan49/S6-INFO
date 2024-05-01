package tpspring.controller.dto;

import tpspring.model.TodoList;

public record NamedDTO(String name) {
    public TodoList getTodoList() {
        TodoList res = new TodoList();
        res.setName(name);
        return res;
    }
}
