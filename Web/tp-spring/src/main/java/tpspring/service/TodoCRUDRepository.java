package tpspring.service;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import tpspring.model.Todo;

import java.util.List;

public interface TodoCRUDRepository extends CrudRepository<Todo, Long>{
    @Query("SELECT t FROM Todo t WHERE t.title LIKE %?1%")
    List<Todo> findByNameFragment(String nameFragment);
}
