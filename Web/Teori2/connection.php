<?php

    $connection = new mysqli("localhost", "root", null, "nwind", 3306);

    if ($connection->connect_error) {
        die("Connection error " . $connection->connect_error);
    }

?>