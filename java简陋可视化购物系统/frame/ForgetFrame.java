// 忘记密码
package frame;

import javax.swing.*;

import pojo1.User;

import java.awt.*;
import java.awt.event.*;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class ForgetFrame extends JFrame implements ActionListener {

    private JTextField idField, emailField, phoneField, passwordField;
    private JButton confirmButton, backButton;
    ArrayList<User> users = User.fromFile("txt\\Users.txt");

    public ForgetFrame() {
        // 设置窗口
        setTitle("忘记密码");
        setSize(1000, 800);
        setResizable(true);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // 创建面板并设置布局
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(6, 8, 100, 50));

        // 添加标签和输入框
        JLabel idLabel = new JLabel("你的ID:");
        idLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(idLabel);
        idField = new JTextField(20);
        idField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(idField);

        JLabel emailLabel = new JLabel("你的邮箱:");
        emailLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(emailLabel);
        emailField = new JTextField(20);
        emailField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(emailField);

        JLabel phoneLabel = new JLabel("你的电话号码:");
        phoneLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(phoneLabel);
        phoneField = new JTextField(20);
        phoneField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(phoneField);

        JLabel passwordLabel = new JLabel("新的密码:");
        passwordLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(passwordLabel);
        passwordField = new JPasswordField(20);
        passwordField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(passwordField);

        // 添加注册和返回按钮
        confirmButton = new JButton("确认");
        confirmButton.setFont(new Font("宋体", Font.PLAIN, 30));
        confirmButton.addActionListener(this);
        panel.add(confirmButton);

        backButton = new JButton("返回");
        backButton.setFont(new Font("宋体", Font.PLAIN, 30));
        backButton.addActionListener(this);
        panel.add(backButton);

        // 将面板添加到窗口中
        add(panel);

        // 显示窗口
        setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == confirmButton) {
            // 获取文本
            int id = Integer.parseInt(idField.getText()) - 1;
            String email = emailField.getText();
            String phone = phoneField.getText();
            String password = passwordField.getText();
            if (id < users.size()) {
                if (users.get(id).getPhonenum().equals(phone) && users.get(id).getEmail().equals(email)) {
                    users.get(id).setPassword(password);
                    JOptionPane.showMessageDialog(this, "修改成功", "提示", JOptionPane.INFORMATION_MESSAGE);
                    try {
                        FileWriter fw = new FileWriter("txt\\Users.txt");
                        fw.write("");
                        fw.close();
                    } catch (IOException er) {
                        System.out.println("发生错误: " + er.getMessage());
                    }
                    
                    for (User us : users) {
                        us.toFile("txt\\Users.txt", true);
                    }
                    dispose();
                    new LoginFrame();
                } else {
                    JOptionPane.showMessageDialog(this, "信息有误", "错误", JOptionPane.ERROR_MESSAGE);
                }
            }

        } else if (e.getSource() == backButton) {
            dispose();
            new LoginFrame();
        }
    }
}