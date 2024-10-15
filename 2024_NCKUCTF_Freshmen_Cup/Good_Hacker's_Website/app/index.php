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
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Good Hacker's Forum</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            line-height: 1.6;
            margin: 0;
            padding: 0;
            background-color: #1a1a1a;
            color: #f0f0f0;
        }
        .container {
            max-width: 800px;
            margin: 40px auto;
            background: #2a2a2a;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 20px rgba(0,0,0,0.3);
        }
        h2 {
            color: #4CAF50;
            text-align: center;
            font-size: 2.5em;
            margin-bottom: 30px;
        }
        img {
            display: block;
            margin: 20px auto;
            border-radius: 10px;
            box-shadow: 0 0 15px rgba(76,175,80,0.3);
        }
        p {
            text-align: center;
            font-size: 1.1em;
        }
        a {
            color: #4CAF50;
            text-decoration: none;
            transition: color 0.3s ease;
        }
        a:hover {
            color: #45a049;
            text-decoration: underline;
        }
        .button {
            display: inline-block;
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border-radius: 5px;
            transition: background-color 0.3s ease;
        }
        .button:hover {
            background-color: #45a049;
            text-decoration: none;
        }
        .avatar {
            width: 150px;
            height: 150px;
            border-radius: 50%;
            object-fit: cover;
            border: 3px solid #4CAF50;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>歡迎來到好駭客的網站！</h2>
        <p>Shell me if you can!</p>
        <img src="images/good_hacker.jpg" alt="Good Hacker" style="width:300px;height:auto;">
        <?php if (isset($_SESSION["username"])): ?>
            <p>Hello, good hacker <strong><?php echo htmlspecialchars(
                $_SESSION["username"]
            ); ?></strong>!</p>

            <?php
            $avatarPath = null;
            if (isset($_SESSION["avatar"])) {
                $avatarPath = $_SESSION["avatar"];
            }
            if ($avatarPath && file_exists($avatarPath)): ?>
                <p>你的頭像：</p>
                <img src="<?php echo htmlspecialchars(
                    $avatarPath
                ); ?>" alt="Avatar" class="avatar">
            <?php else: ?>
                <p>您還沒有上傳頭像！</p>
            <?php endif;
            ?>

            <p><a href="upload.php" class="button">上傳頭像</a></p>
            <p><a href="reset.php" class="button">更改密碼</a></p>
            <p><a href="logout.php" class="button">登出</a></p>
        <?php else: ?>
            <p>您尚未登入。請先<a href="login.php">登入</a></p>
        <?php endif; ?>
    </div>
</body>
</html>
