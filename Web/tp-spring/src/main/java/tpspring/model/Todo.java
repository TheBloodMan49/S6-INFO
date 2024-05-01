package tpspring.model;

import java.util.ArrayList;
import java.util.List;

import jakarta.persistence.*;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@Entity
public class Todo {
	@Id
	@GeneratedValue
	protected long id;
	protected String title;
	protected String description;
	@ElementCollection
	protected List<Category> categories;
	@ManyToOne
	protected TodoList list;

	protected String owner;

	/**
	 * Temporary constructor for TP1
	 */
	public Todo(long id, String title) {
		this.id = id;
		this.title = title;
		description = "";
		categories = new ArrayList<>();
	}

	public Todo(long id, String title, String description, List<Category> categories, TodoList list, String owner) {
		this.id = id;
		this.title = title;
		this.description = description;
		this.categories = categories;
		this.list = list;
		this.owner = owner;
	}

    @Override
    public String toString() {
        return "Todo [id=" + id + ", title=" + title
                + ", description=" + description + ", categories=" + categories + "]";
    }

	@Override
	public int hashCode() {
		return (int) id;
	}
}
