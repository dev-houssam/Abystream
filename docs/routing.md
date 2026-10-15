# Routing

Le routage de abystream s'inspire d'un module go :

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


