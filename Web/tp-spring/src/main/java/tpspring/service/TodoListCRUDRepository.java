package tpspring.service;

import org.springframework.data.repository.CrudRepository;
import tpspring.model.TodoList;


public interface TodoListCRUDRepository extends CrudRepository<TodoList, Long>{
}
