// 登录界面
package frame;

import javax.swing.*;

import pojo1.User;

import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class LoginFrame extends JFrame implements ActionListener {
    private JTextField phoneField;
    private JPasswordField passwordField;
    private JButton loginButton, registerButton, exitButton, forgetButton;
    ArrayList<User> users = User.fromFile("txt\\Users.txt");

    public LoginFrame() {
        // 设置窗口
        setTitle("登录");
        setSize(1000, 800);
        setResizable(true);
        setLocationRelativeTo(null);

        // 创建面板并设置布局
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(8, 10, 50, 25));

        // 添加标签和输入框
        JLabel phoneLabel = new JLabel("电话号码/ID：");
        phoneLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(phoneLabel);
        phoneField = new JTextField(20);
        phoneField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(phoneField);

        JLabel passwordLabel = new JLabel("密码：");
        passwordLabel.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(passwordLabel);
        passwordField = new JPasswordField(20);
        passwordField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(passwordField);

        // 创建按钮并添加事件监听器
        loginButton = new JButton("登录");
        loginButton.setFont(new Font("宋体", Font.PLAIN, 30));
        loginButton.addActionListener(this);
        panel.add(loginButton);

        registerButton = new JButton("注册");
        registerButton.setFont(new Font("宋体", Font.PLAIN, 30));
        registerButton.addActionListener(this);
        panel.add(registerButton);

        forgetButton = new JButton("忘记密码");
        forgetButton.setFont(new Font("宋体", Font.PLAIN, 30));
        forgetButton.addActionListener(this);
        panel.add(forgetButton);

        exitButton = new JButton("退出");
        exitButton.setFont(new Font("宋体", Font.PLAIN, 30));
        exitButton.addActionListener(this);
        panel.add(exitButton);

        // 添加面板到窗口
        getContentPane().add(panel);

        // 设置窗口可见
        setVisible(true);
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == loginButton) {
            // 获取电话号码和密码
            String phone = phoneField.getText();
            String password = new String(passwordField.getPassword());

            // 登录验证
            boolean success = false;
            User u = null;
            if (phone.length() == 1) {
                for (User uu : users) {
                    if (uu.getId() == Integer.parseInt(phone) && uu.getPassword().equals(password)) {
                        success = true;
                        u = uu;
                        break;
                    }
                }
            } else {
                for (User uu : users) {
                    if (uu.getPhonenum().equals(phone) && uu.getPassword().equals(password)) {
                        success = true;
                        u = uu;
                        break;
                    }
                }
            }

            // 弹窗提示登录结果
            if (success) {
                JOptionPane.showMessageDialog(this, "登录成功！", "提示", JOptionPane.INFORMATION_MESSAGE);
                dispose();
                new MainFrame(u, null);
            } else {
                JOptionPane.showMessageDialog(this, "电话号码或密码错误，请重新输入！", "提示", JOptionPane.ERROR_MESSAGE);
            }
        } else if (e.getSource() == registerButton) {
            dispose();
            new RegisterFrame();
        } else if (e.getSource() == forgetButton) {
            dispose();
            new ForgetFrame();
        } else if (e.getSource() == exitButton) {
            // 点击退出按钮，关闭窗口
            JOptionPane.showMessageDialog(this, "欢迎下次使用", "提示", JOptionPane.INFORMATION_MESSAGE);
            System.exit(0); // 退出
        }
    }

    public static void main(String[] args) {
        new LoginFrame();
    }
}