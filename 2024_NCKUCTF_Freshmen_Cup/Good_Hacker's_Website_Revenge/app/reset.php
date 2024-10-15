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

function changePassword($conn, $username, $newPassword)
{
    // Hash the new password
    $hashedPassword = password_hash($newPassword, PASSWORD_DEFAULT);

    // Check if the user exists
    $stmt = $conn->prepare("SELECT username FROM users WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $result = $stmt->get_result();

    if ($result->num_rows > 0) {
        // User exists, update the password
        $stmt = $conn->prepare(
            "UPDATE users SET password = ? WHERE username = ?"
        );
        $stmt->bind_param("ss", $hashedPassword, $username);
        return $stmt->execute();
    } else {
        // User does not exist, create a new user
        $stmt = $conn->prepare(
            "INSERT INTO users (username, password) VALUES (?, ?)"
        );
        $stmt->bind_param("ss", $username, $hashedPassword);
        return $stmt->execute();
    }
}

if (
    $_SERVER["REQUEST_METHOD"] == "POST" &&
    isset($_POST["change_password"]) &&
    isset($_POST["username"]) &&
    isset($_POST["new_password"]) &&
    isset($_POST["confirm_password"])
) {
    $username = $_POST["username"];
    if ($_POST["new_password"] === $_POST["confirm_password"]) {
        if (changePassword($conn, $username, $_POST["new_password"])) {
            $message =
                "<div class='alert alert-success'>Password changed successfully!</div>";
        } else {
            $message =
                "<div class='alert alert-danger'>Password change failed!</div>";
        }
    } else {
        $message =
            "<div class='alert alert-warning'>Passwords do not match!</div>";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>更改密碼</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body {
            background-color: #f8f9fa;
        }
        .container {
            max-width: 400px;
            margin-top: 50px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2 class="text-center mb-4">更改密碼</h2>
        <?php if (isset($message)) {
            echo $message;
        } ?>
        <?php if (isset($_SESSION["username"])): ?>
        <form method="post">
            <input type="hidden" name="username" value="<?php echo $_SESSION[
                "username"
            ]; ?>">
            <div class="form-group">
                <label for="new_password">新密碼：</label>
                <input type="password" class="form-control" id="new_password" name="new_password" required>
            </div>
            <div class="form-group">
                <label for="confirm_password">確認新密碼：</label>
                <input type="password" class="form-control" id="confirm_password" name="confirm_password" required>
            </div>
            <button type="submit" name="change_password" class="btn btn-primary btn-block">更改密碼</button>
        </form>
        <p class="mt-3 text-center"><a href="index.php">返回到首頁</a></p>
        <?php else: ?>
        <div class="alert alert-info">您必須登入才能改密碼！</div>
        <p class="text-center"><a href="login.php" class="btn btn-link">立即登入</a></p>
        <?php endif; ?>
    </div>
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.3/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
