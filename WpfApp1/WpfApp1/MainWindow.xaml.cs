using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Int32 _baudRate;
        private String _pName;
        public MainWindow()
        {
            InitializeComponent();
            foreach (String s in SerialPort.GetPortNames())
                portList.Items.Add(s);
        }

        private void refresh_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void ready_Click(object sender, RoutedEventArgs e)
        {
            
            try
            {
                String bval = baudList.SelectedItem.ToString();
                _baudRate = Int32.Parse(bval);
                _pName = portList.SelectedItem.ToString();
                tB1.Text = bval;
                tB2.Text = _pName;
                _spData(_pName, _baudRate);
            }
            catch (Exception ex)
            {
                resTBox.AppendText(ex.ToString());
            }
            
        }

        private void _spData(String s, Int32 x)
        {
            var SP = new SerialPort(_pName, _baudRate);
            SP.Open();
            SP.DataReceived += sp_recieve;
        }
        //creating a delegate
        public delegate void OutputResultDelegate(String myString);
        void sp_recieve(Object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort _SP = (SerialPort)sender;
            String data = _SP.ReadLine();
            resTBox.Dispatcher.Invoke(new OutputResultDelegate(setTxt), new Object[] { data });
            _SP.Close();
        }

        private void setTxt(String s)
        {
            resTBox.AppendText(Environment.NewLine+s);
        }
    }
}
