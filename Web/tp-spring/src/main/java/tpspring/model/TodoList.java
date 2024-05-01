package tpspring.model;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;
import lombok.Data;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

@Entity
@NoArgsConstructor
@Getter
@Setter
public class TodoList {
	@Id
	@GeneratedValue
	private long id;
	private String name;
	@OneToMany
	private List<Todo> todos;
	private String owner;

	public TodoList(final String name) {
		super();
		this.name = name;
		todos = new ArrayList<>();
	}
}
