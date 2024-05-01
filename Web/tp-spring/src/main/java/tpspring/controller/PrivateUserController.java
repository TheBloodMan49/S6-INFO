package tpspring.controller;

import java.security.Principal;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpSession;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PatchMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;
import tpspring.controller.dto.UserDTO;
import tpspring.service.UserService;

@RestController
@RequestMapping("api/private/user")
@AllArgsConstructor
@CrossOrigin
public class PrivateUserController {
    private final UserService userService;

    @GetMapping()
    public String hello(final Principal user) {
        return user.getName();
    }

    @DeleteMapping()
    public void delAccount(final Principal principal, final HttpServletRequest request) {
        if(principal == null) {
            return;
        }
        userService.delAccount(principal.getName());
        // Logging out the current user
        SecurityContextHolder.clearContext();
        final HttpSession session = request.getSession(false);
        if(session != null) {
            session.invalidate();
        }
    }

    @PostMapping("out")
    public void logout() {
        try {
            if(!userService.logout()) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Cannot log out");
            }
        }catch(final ServletException | IllegalStateException ex) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Cannot log out");
        }
    }

}
