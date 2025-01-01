package pojo1;

import java.io.*;
import java.util.*;

public class User {
    private int id; // 用户编号
    private String username; // 用户姓名
    private String email;// 用户邮箱
    private String phonenum; // 用户电话号码
    private String address; // 用户地址
    private String password; // 用户密码

    public User(int id, String username, String email, String phonenum, String address, String password) {
        this.id = id;
        this.username = username;
        this.email = email;
        this.phonenum = phonenum;
        this.address = address;
        this.password = password;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPhonenum() {
        return phonenum;
    }

    public void setPhonenum(String phonenum) {
        this.phonenum = phonenum;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
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

    public static ArrayList<User> fromFile(String filename) {
        CheckFile(filename);
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            ArrayList<User> userlist = new ArrayList<User>();
            String line;
            while ((line = reader.readLine()) != null) {
                int id = Integer.parseInt(line.substring(5));
                String name = reader.readLine().substring(4);
                String email = reader.readLine().substring(3);
                String phonenum = reader.readLine().substring(5);
                String address = reader.readLine().substring(3);
                String password = reader.readLine().substring(3);
                reader.readLine();

                User u = new User(id, name, email, phonenum, address, password);
                userlist.add(u);
            }
            reader.close();
            return userlist;
        } catch (NumberFormatException | IOException e) {
            System.out.println("读写错误，请重新尝试");
            e.printStackTrace();
            return null;
        }
    }

    public void toFile(String filename,boolean flag) {
        CheckFile(filename);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, flag))) {
            writer.write("用户ID:"+ id + "\n");
            writer.write("用户名:" + username + "\n");
            writer.write("邮箱:" + email + "\n");
            writer.write("电话号码:" + phonenum + "\n");
            writer.write("地址:" + address + "\n");
            writer.write("密码:" + password + "\n");
            writer.write("\n");

            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("读写错误");
        }
    }
}
