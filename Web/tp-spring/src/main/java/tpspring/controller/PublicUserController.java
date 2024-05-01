package tpspring.controller;

import jakarta.servlet.ServletException;
import lombok.AllArgsConstructor;
import org.apache.coyote.Response;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.server.ResponseStatusException;
import tpspring.controller.dto.UserDTO;
import tpspring.service.UserService;

@RestController
@CrossOrigin
@AllArgsConstructor
@RequestMapping("api/v2/public/user")
public class PublicUserController {
    private final UserService userService;

    @PostMapping(value="new", consumes= MediaType.APPLICATION_JSON_VALUE)
    public void newAccount(@RequestBody final UserDTO user) {
        try  {
            userService.newAccount(user.login(), user.password());
        } catch (final IllegalArgumentException e) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, e.getMessage());
        }
    }

    @PostMapping(value="login", consumes= MediaType.APPLICATION_JSON_VALUE)
    public void login(@RequestBody final UserDTO user) {
        try {
            final boolean logged = userService.login(user.login(), user.password());

            if (!logged) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Already logged in");
            }
        } catch (final ServletException e) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Can't log in");
        }
    }
}
