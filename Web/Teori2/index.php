<?php
    require("./connection.php");
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Products</title>
</head>
<body>
    <table border="1">
        <thead>
            <tr>
                <th>Product ID</th>
                <th>Product Name</th>
                <th>Category Name</th>
                <th>Company Name</th>
                <th>Unit Price</th>
            </tr>
        </thead>
        <tbody>
            <?php
                $query = $connection->query("
                    SELECT 
                        products.ProductID,
                        products.ProductName,
                        categories.CategoryName,
                        suppliers.CompanyName,
                        products.UnitPrice
                    FROM products
                    JOIN categories
                        ON categories.CategoryID = products.CategoryID
                    JOIN suppliers
                        ON suppliers.SupplierID = products.SupplierID;
                ");

                foreach ($query->fetch_all(MYSQLI_ASSOC) as $row) {
            ?>
                <tr>
                    <td><?=$row["ProductID"]?></td>
                    <td><?=$row["ProductName"]?></td>
                    <td><?=$row["CategoryName"]?></td>
                    <td><?=$row["CompanyName"]?></td>
                    <td><?=$row["UnitPrice"]?></td>
                </tr>
            <?php
                }
            ?>
        </tbody>
    </table>
</body>
</html>