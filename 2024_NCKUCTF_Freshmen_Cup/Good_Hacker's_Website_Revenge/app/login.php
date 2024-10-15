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

function login($conn, $username, $password)
{
    $stmt = $conn->prepare("SELECT password FROM users WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $result = $stmt->get_result();
    if ($result->num_rows > 0) {
        $user = $result->fetch_assoc();
        if (password_verify($password, $user["password"])) {
            $_SESSION["username"] = $username;
            return true;
        }
    }
    return false;
}

if (
    $_SERVER["REQUEST_METHOD"] == "POST" &&
    isset($_POST["login"]) &&
    isset($_POST["username"]) &&
    isset($_POST["password"])
) {
    if (login($conn, $_POST["username"], $_POST["password"])) {
        header("Location: index.php");
        exit();
    } else {
        $error_message = "登入失敗，請檢查您的用戶名和密碼。";
    }
}
?>

<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>登入</title>
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
        .login-container {
            background-color: white;
            padding: 2rem;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            width: 300px;
        }
        h2 {
            text-align: center;
            color: #333;
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
            transition: background-color 0.3s;
        }
        input[type="submit"]:hover {
            background-color: #45a049;
        }
        .error-message {
            color: red;
            text-align: center;
            margin-bottom: 10px;
        }
        p {
            text-align: center;
            margin-top: 20px;
        }
        a {
            color: #4CAF50;
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
    </style>
</head>
<body>
    <div class="login-container">
        <h2>登入</h2>
        <?php if (isset($error_message)): ?>
            <p class="error-message"><?php echo $error_message; ?></p>
        <?php endif; ?>
        <form method="post">
            <input type="text" name="username" placeholder="用戶名" required>
            <input type="password" name="password" placeholder="密碼" required>
            <input type="submit" name="login" value="登入">
        </form>
        <p>還沒有帳號？ <a href="register.php">立即註冊</a></p>
    </div>
</body>
</html>
