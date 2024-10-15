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

function uploadAvatar($username, $avatarFile)
{
    $targetDir = "uploads/";
    $targetFile = $targetDir . $username . "_" . basename($avatarFile["name"]);

    // Check if file is a valid image
    $check = getimagesize($avatarFile["tmp_name"]);
    if ($check === false) {
        echo "File is not an image.\n";
        return false;
    }
    // Check if it's larger than 2 MB
    if ($avatarFile["size"] > 2 * 1024 * 1024) {
        echo "File is larger than 2MB. Too large.\n";
        return false;
    }
    // Check invalid file extensions
    $imageFileType = pathinfo($avatarFile["name"], PATHINFO_EXTENSION);
    $bannedExtensions = [
        "php",
        "php3",
        "php4",
        "php5",
        "php7",
        "phtml",
        "phar",
        "inc",
        "hphp",
        "ctp",
        "module",
    ];

    if (in_array($imageFileType, $bannedExtensions)) {
        echo "Invalid file extension!\n";
        return false;
    }
    // Don't allow htaccess

    if (strpos($avatarFile["name"], ".htaccess") !== false) {
        echo "Invalid file name.\n";
        return false;
    }

    // Allow only jpg, png file types using magic type
    $finfo = finfo_open(FILEINFO_MIME_TYPE);
    $mime = finfo_file($finfo, $avatarFile["tmp_name"]);
    if (
        $mime !== "image/jpeg" &&
        $mime !== "image/png" &&
        $mime !== "image/gif"
    ) {
        echo "Invalid jpeg/png/gif file!\n";
        return false;
    }

    // Move the file to the target directory
    if (move_uploaded_file($avatarFile["tmp_name"], $targetFile)) {
        echo "Avatar uploaded to " . $targetFile . " successfully!\n";
        $_SESSION["avatar"] = $targetFile;
        return true;
    } else {
        echo "There was an error uploading the avatar.\n";
        return false;
    }
}

if (
    $_SERVER["REQUEST_METHOD"] == "POST" &&
    isset($_FILES["avatar"]) &&
    isset($_SESSION["username"])
) {
    uploadAvatar($_SESSION["username"], $_FILES["avatar"]);
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>上傳頭像</title>
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
            padding: 30px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            text-align: center;
        }
        h2 {
            color: #333;
        }
        form {
            margin-top: 20px;
        }
        input[type="file"] {
            display: none;
        }
        label {
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            display: inline-block;
            margin-bottom: 10px;
        }
        input[type="submit"] {
            background-color: #008CBA;
            border: none;
            color: white;
            padding: 10px 20px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            border-radius: 4px;
        }
        p {
            color: #666;
        }
        a {
            color: #008CBA;
            text-decoration: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>上傳頭像</h2>
        <?php if (isset($_SESSION["username"])): ?>
        <form method="post" enctype="multipart/form-data">
            <label for="avatar">選擇你的頭像檔案（接受 jpg / gif / png 格式）</label>
            <input type="file" id="avatar" name="avatar" required>
            <br>
            <input type="submit" value="Upload Avatar">
        </form>
        <?php else: ?>
        <p>您必須登入才能上傳頭像！</p>
        <p><a href="login.php">立即登入</a></p>
        <?php endif; ?>
    </div>
</body>
</html>
