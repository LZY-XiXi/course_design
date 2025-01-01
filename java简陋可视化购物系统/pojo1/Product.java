package pojo1;

import java.io.*;
import java.util.*;

public class Product {
    private int id; // 商品编号
    private String name;// 商品名称
    private String category; // 商品类别
    private int amount; // 商品数量
    private double price; // 商品单价
    private double totle; // 商品总价

    public Product(int id, String name, String category, int amount, double price) {
        this.id = id;
        this.name = name;
        this.category = category;
        this.amount = amount;
        this.price = price;
        this.totle = amount * price;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public double getTotle() {
        return totle;
    }

    public static void CheckFile(String filename) {
        File file = new File(filename);
        if (!file.exists()) {
            try {
                // 创建新文件
                file.createNewFile();
                System.out.println("文件不存在，已自动创建");
            } catch (IOException e) {
                System.out.println("创建文件时发生错误！");
                e.printStackTrace();
            }
        }
    }

    public static ArrayList<Product> fromFile(String filename) {
        CheckFile(filename);
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            ArrayList<Product> productlist = new ArrayList<Product>();
            String line;
            while ((line = reader.readLine()) != null) {
                int id = Integer.parseInt(line.substring(5));
                String name = reader.readLine().substring(5);
                String category = reader.readLine().substring(5);
                int amount = Integer.parseInt(reader.readLine().substring(5));
                double price = Double.parseDouble(reader.readLine().substring(5));
                reader.readLine();
                Product p = new Product(id, name, category, amount, price);
                productlist.add(p);
            }
            reader.close();
            return productlist;
        } catch (NumberFormatException | IOException e) {
            System.out.println("读写错误，请重新尝试");
            e.printStackTrace();
            return null;
        }
    }

    
    public void toFile(String filename, boolean flag) {
        CheckFile(filename);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, flag))) {
            writer.write("商品id:" + id + "\n");
            writer.write("商品名称:" + name + "\n");
            writer.write("商品类别:" + category + "\n");
            writer.write("商品数量:" + amount + "\n");
            writer.write("商品单价:" + price + "\n");

            writer.write("\n");

            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("读写错误");
        }
    }

    public void toFile_Order(String filename, boolean flag) {
        CheckFile(filename);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, flag))) {
            writer.write("商品id:" + id + "\n");
            writer.write("商品名称:" + name + "\n");
            writer.write("商品类别:" + category + "\n");
            writer.write("商品数量:" + amount + "\n");
            writer.write("商品单价:" + price + "\n");

            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("读写错误");
        }
    }
}
