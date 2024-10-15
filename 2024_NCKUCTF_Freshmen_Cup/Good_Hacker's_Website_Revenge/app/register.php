<?php
session_start();

$host = "mysql";
$user = "root";
$pass = "password"; 
$dbname = "mydb";

$conn = new mysqli($host, $user, $pass, $dbname);

// Create users table if it doesn't exist (only storing username and password)
$conn->query("CREATE TABLE IF NOT EXISTS users (
    username VARCHAR(50) NOT NULL PRIMARY KEY,
    password VARCHAR(255) NOT NULL
)");

function register($conn, $username, $password)
{
    // Validate if the username is alphanumeric
    if (!ctype_alnum($username)) {
        echo "<div class='error'>Username must be all alphanumeric!</div>";
        return false;
    }

    // Check if the username already exists
    $stmt = $conn->prepare("SELECT username FROM users WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->store_result();

    if ($stmt->num_rows > 0) {
        echo "<div class='error'>Username already exists!</div>";
        return false;
    }

    // Proceed with registration
    $hashedPassword = password_hash($password, PASSWORD_DEFAULT);
    $stmt = $conn->prepare(
        "INSERT INTO users (username, password) VALUES (?, ?)"
    );
    $stmt->bind_param("ss", $username, $hashedPassword);
    return $stmt->execute();
}

if (
    $_SERVER["REQUEST_METHOD"] == "POST" &&
    isset($_POST["register"]) &&
    isset($_POST["username"]) &&
    isset($_POST["password"])
) {
    if (register($conn, $_POST["username"], $_POST["password"])) {
        echo "<div class='success'>Registration success!</div>";
    } else {
        echo "<div class='error'>Registration failed!</div>";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>註冊帳號</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .container {
            background-color: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        h2 {
            color: #333;
            text-align: center;
        }
        form {
            display: flex;
            flex-direction: column;
        }
        input {
            margin: 10px 0;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
        }
        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }
        .error {
            color: red;
            margin-bottom: 10px;
        }
        .success {
            color: green;
            margin-bottom: 10px;
        }
        p {
            text-align: center;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>註冊帳號</h2>
        <form method="post">
            <input type="text" name="username" placeholder="Username" required>
            <input type="password" name="password" placeholder="Password" required>
            <input type="submit" name="register" value="Register">
        </form>
        <p>已經有帳號了！<a href="login.php">立即登入</a></p>
    </div>
</body>
</html>
