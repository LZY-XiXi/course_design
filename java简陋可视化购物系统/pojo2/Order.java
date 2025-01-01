package pojo2;

import java.io.*;
import java.util.*;

import pojo1.*;

public class Order {
    private final static String STR = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    private final static int LEN = 15;
    User u = null;
    Trolley t = null;
    String txt = null;

    Order(User u, Trolley t, String txt) {
        this.u = u;
        this.t = t;
        this.txt = txt;
    }

    public Order(Trolley t, String txt) {
        this.t = t;
        this.txt = txt;
    }

    public User getU() {
        return u;
    }

    public void setU(User u) {
        this.u = u;
    }

    public Trolley getT() {
        return t;
    }

    public void setT(Trolley t) {
        this.t = t;
    }

    public String getTxt() {
        return txt;
    }

    public void setTxt(String txt) {
        this.txt = txt;
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

    public static ArrayList<Order> fromFile(String filename) {
        CheckFile(filename);
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            ArrayList<Order> ordList = new ArrayList<Order>();
            ArrayList<Product> productlist = new ArrayList<Product>();
            String line;
            while ((line = reader.readLine()) != null) {
                if (!line.startsWith("订单号")) {
                    int id = Integer.parseInt(line.substring(5));
                    String name = reader.readLine().substring(5);
                    String category = reader.readLine().substring(5);
                    int amount = Integer.parseInt(reader.readLine().substring(5));
                    double price = Double.parseDouble(reader.readLine().substring(5));
                    Product p = new Product(id, name, category, amount, price);
                    productlist.add(p);
                } else {
                    String txt = line.substring(4);
                    Trolley tro = new Trolley(new ArrayList<Product>(productlist));
                    Order o = new Order(tro, txt);
                    ordList.add(o);
                    reader.readLine();
                    productlist.clear();
                }
            }
            reader.close();
            return ordList;
        } catch (NumberFormatException | IOException e) {
            System.out.println("读写错误，请重新尝试");
            e.printStackTrace();
            return null;
        }
    }

    public void toFile(String filename, boolean flag) {
        CheckFile(filename);
        for (Product pro : this.t.getProlist()) {
            pro.toFile_Order(filename, flag);
        }
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, flag))) {
            writer.write("订单号：" + this.txt + "\n\n");
        } catch (IOException e) {
            System.out.println("读写错误，请重新尝试");
            e.printStackTrace();
        }
    }

    public static String randomID() {
        Random random = new Random();
        StringBuilder sb = new StringBuilder(LEN);
        for (int i = 0; i < LEN; i++) {
            int randomIndex = random.nextInt(STR.length());
            char randomChar = STR.charAt(randomIndex);
            sb.append(randomChar);
        }
        return sb.toString();
    }
}