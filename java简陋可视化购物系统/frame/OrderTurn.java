package frame;

import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import pojo1.*;
import pojo2.Order;
import pojo2.Trolley;

public class OrderTurn extends JFrame implements ActionListener {
    private JTextField textField;
    private JButton confirmButton;
    private JButton backButton;
    User u = null;
    ArrayList<Order> ordList = Order.fromFile("txt\\Orders.txt");

    OrderTurn(User u) {
        this.u = u;
        for (Order ord : ordList) {
            ord.setU(u);
        }
        setTitle("订单查询");
        setSize(1000, 800);
        setResizable(true);
        setLocationRelativeTo(null);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(8, 10, 50, 25));

        JLabel label = new JLabel("请输入订单号");
        label.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(label);
        textField = new JTextField(20);
        textField.setFont(new Font("宋体", Font.PLAIN, 30));
        panel.add(textField);

        confirmButton = new JButton("确认");
        confirmButton.setFont(new Font("宋体", Font.PLAIN, 30));
        confirmButton.addActionListener(this);
        panel.add(confirmButton);

        backButton = new JButton("返回");
        backButton.setFont(new Font("宋体", Font.PLAIN, 30));
        backButton.addActionListener(this);
        panel.add(backButton);

        add(panel);

        // 设置窗口可见性
        setVisible(true);

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == confirmButton) {
            String text = textField.getText();
            boolean flag = true;
            for (Order ord : ordList) {
                if (ord.getTxt().equals(text)) {
                    flag = false;
                    dispose();
                    new OrderFrame(u, ord.getT(), false,ord.getTxt());
                }
            }
            if (flag) {
                JOptionPane.showMessageDialog(this, "订单不存在", "提示", JOptionPane.INFORMATION_MESSAGE);
            }
        } else if (e.getSource() == backButton) {
            dispose();
            new MainFrame(u, new Trolley(null));
        }
    }
}
