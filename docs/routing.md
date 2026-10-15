# Routing

Le routage de abystream s'inspire d'un module go pour ceux qui sont familier à ce langage :
```go

package main

import "github.com/gin-gonic/gin"

func main() {
    router := gin.Default()

    // Création d'un groupe principal (Node racine du groupe)
    v1 := router.Group("/api/v1")
    {
        // Sous-routes rattachées à ce groupe
        v1.GET("/users", listUsers)
        v1.POST("/users", createUser)

        // Sous-groupe (imbriqué) pour l'administration
        admin := v1.Group("/admin")
        {
            admin.DELETE("/users/:id", deleteUser)
        }
    }

    router.Run(":8080")
}

```

Maintenant voici, un exemple en Abystream de MATL-ABC: 

```c
NodeGroup* build_routes() {
    return new NodeGroup(
        "",
        new GET(route("/"), (Request* req, Response* res) => {
            HomeController* home = new HomeController();
            home.index(req, res);
        }),
        new GET(route("/posts/new"), (Request* req, Response* res) => {
            PostController* posts = new PostController();
            posts.form(req, res);
        }),
        new POST(route("/posts"), (Request* req, Response* res) => {
            PostController* posts = new PostController();
            posts.create(req, res);
        }),
        new GET(route("^/posts/([0-9]+)$"), (Request* req, Response* res) => {
            PostController* posts = new PostController();
            posts.show(req, res);
        }),
        new POST(route("^/posts/([0-9]+)/delete$"), (Request* req, Response* res) => {
            PostController* posts = new PostController();
            posts.remove(req, res);
        })
    );
}
```
