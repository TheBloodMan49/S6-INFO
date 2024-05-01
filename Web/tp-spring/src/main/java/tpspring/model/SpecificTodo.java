package tpspring.model;

import jakarta.persistence.Entity;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

@Entity
@NoArgsConstructor
@Data
public class SpecificTodo extends Todo {
	private String specificAttr;

	public int hashCode() {
		return super.hashCode();
	}
}
