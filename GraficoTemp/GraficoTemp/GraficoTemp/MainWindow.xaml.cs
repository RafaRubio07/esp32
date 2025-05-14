using System;
using System.Net.Http;
using System.Threading.Tasks;
using System.Windows;
using OxyPlot;
using OxyPlot.Series;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;

namespace GraficoTemp
{

    public partial class MainWindow : Window
    {
        private readonly LineSeries tempSeries;
        private readonly PlotModel plotModel;
        private readonly string apiUrl = "https://api.thingspeak.com/channels/2954614/fields/1.json?api_key=38J48ICFTMI0G6UB&results=10\"";
        public MainWindow()
        {
            InitializeComponent();
            plotModel = new PlotModel { Title = "Temperatura" };
            tempSeries = new LineSeries { Title = "Temperatura", MarkerType = MarkerType.Circle };
            plotModel.Series.Add(tempSeries);
            tempChart.Model = plotModel;
            FetchTemperatureData();
        }

        private async void FetchTemperatureData() 
        {
            try
            {
                using HttpClient client = new();
                var response = await client.GetStringAsync(apiUrl);
                var parsedData = ParseTemperatureData(response);
                UpdateChart(parsedData);
            }
            catch (Exception ex) 
            {
                MessageBox.Show($"Erro ao obter dados: {ex.Message}");
            }
        }

        private List<double> ParseTemperatureData(string json)
        {
            var datalist = new List<double>();
            var jsonObject = JObject.Parse(json);
            foreach (var item in jsonObject["feeds"])
            {
                if (double.TryParse(item["field1"].ToString(), out double value))
                {
                    datalist.Add(value);
                }
            }
            return datalist;
        }
        
        private void UpdateChart(List<double> newData)
        {
            tempSeries.Points.Clear();
            for (int i = 0; i < newData.Count; i++)
            {
                tempSeries.Points.Add(new OxyPlot.DataPoint(i, newData[i]));
            }
            plotModel.InvalidatePlot(true);
        }
    }
}