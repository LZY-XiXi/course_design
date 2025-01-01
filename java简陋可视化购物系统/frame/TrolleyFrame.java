// 购物车页面
package frame;

import pojo2.Trolley;

import javax.swing.*;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

import pojo1.*;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

public class TrolleyFrame extends JFrame implements ActionListener {

    private JLabel totalLabel, totalPriceLabel;
    private JTable cartTable;
    private JButton clearButton, checkoutButton, returnButton;
    private Trolley tro = null;
    User u = null;
    ArrayList<Object[]> dataList = new ArrayList<Object[]>();

    private DefaultTableModel tableModel;

    public TrolleyFrame(User u, Trolley tro) {
        // 初始化
        this.u = u;
        this.tro = tro;

        // 设置窗口
        setTitle("购物车");
        setSize(1000, 800);
        setResizable(true); // 改变窗口大小
        setLocationRelativeTo(null); // 将窗口位置设置为屏幕中央
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        totalLabel = new JLabel("总价：");
        totalLabel.setFont(new Font("宋体", Font.PLAIN, 20));
        totalPriceLabel = new JLabel("$0.0");
        totalPriceLabel.setFont(new Font("宋体", Font.PLAIN, 20));
        tableModel = new DefaultTableModel();

        String[] columnNames = { "编号", "名称", "类型", "数量", "单价" };
        if (tro != null && tro.getProlist() != null) {
            for (Product product : tro.getProlist()) {
                dataList.add(new Object[] { product.getId(), product.getName(),
                        product.getCategory(), product.getAmount(), product.getPrice() });
            }
        }

        // 将ArrayList数据转换为二维数组
        Object[][] data = dataList.toArray(new Object[dataList.size()][]);
        tableModel.setDataVector(data, columnNames);

        // 创建表格，并添加TableModelListener
        cartTable = new JTable(tableModel);
        cartTable.getTableHeader().setFont(new Font("宋体", Font.PLAIN, 30));
        cartTable.setFont(new Font("宋体", Font.PLAIN, 20));
        cartTable.setRowHeight(30); // 将行高设置为30像素

        cartTable.getModel().addTableModelListener(new TableModelListener() {
            @Override
            public void tableChanged(TableModelEvent e) {
                if (e.getType() == TableModelEvent.UPDATE) {
                    updateTotalPrice();
                    int row = e.getFirstRow();
                    int amount = Integer.parseInt(tableModel.getValueAt(row, 3).toString());
                    tro.getProlist().get(row).setAmount(amount);
                    if (amount == 0) {
                        tableModel.removeRow(row);
                        tro.getProlist().remove(row);
                    }
                }
            }
        });

        // 创建添加、删除、清空、结账和返回按钮
        clearButton = new JButton("清空购物车");
        clearButton.setFont(new Font("宋体", Font.PLAIN, 30));
        clearButton.addActionListener(this);

        checkoutButton = new JButton("结账生成订单");
        checkoutButton.setFont(new Font("宋体", Font.PLAIN, 30));
        checkoutButton.addActionListener(this);

        returnButton = new JButton("返回");
        returnButton.setFont(new Font("宋体", Font.PLAIN, 30));
        returnButton.addActionListener(this);

        updateTotalPrice();

        // 创建界面布局
        JPanel cartPanel = new JPanel(new BorderLayout());

        JScrollPane scrollPane = new JScrollPane(cartTable);
        cartPanel.add(scrollPane, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel(new GridLayout(3, 3));
        buttonPanel.add(clearButton);
        buttonPanel.add(checkoutButton);
        buttonPanel.add(returnButton);

        JPanel totalPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        totalPanel.add(totalLabel);
        totalPanel.add(totalPriceLabel);

        JPanel mainPanel = new JPanel(new BorderLayout());
        mainPanel.add(cartPanel, BorderLayout.CENTER);
        mainPanel.add(buttonPanel, BorderLayout.SOUTH);
        mainPanel.add(totalPanel, BorderLayout.EAST);

        // 将界面添加到窗口中
        add(mainPanel);

        // 显示窗口
        setVisible(true);
    }

    protected JFrame getProlist() {
        return null;
    }

    // 更新购物车总价显示
    private void updateTotalPrice() {
        double totalPrice = 0;
        for (int i = 0; i < tableModel.getRowCount(); i++) {
            int amount = Integer.parseInt(tableModel.getValueAt(i, 3).toString());
            double price = (double) tableModel.getValueAt(i, 4);
            totalPrice += amount * price;
            if (amount == 0) {
                dataList.remove(i);
            }
        }
        totalPriceLabel.setText(String.format("$%.2f", totalPrice));
        totalPriceLabel.setFont(new Font("宋体", Font.PLAIN, 20));
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == clearButton) {
            if (tro != null) {
                tro.getProlist().clear();
            }
            JOptionPane.showMessageDialog(this, "清空完成", "提示", JOptionPane.INFORMATION_MESSAGE);
            dispose();
            new TrolleyFrame(u, tro);
        } else if (e.getSource() == checkoutButton) {
            boolean flag = true;
            if (tro == null) {
                flag = false;
            } else if (tro.getProlist() == null || tro.getProlist().size() == 0) {
                flag = false;
            }
            if (flag) {
                dispose();
                new OrderFrame(u, tro,true,null);
            } else {
                JOptionPane.showMessageDialog(this, "购物车内无商品，请添加后再尝试", "提示", JOptionPane.ERROR_MESSAGE);
            }

        } else if (e.getSource() == returnButton) {
            dispose();
            new MainFrame(u, tro);
        }
    }
}