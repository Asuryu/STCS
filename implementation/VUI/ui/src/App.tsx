import React, { useEffect, useState, useRef } from "react";
import {
  CartesianGrid,
  Line,
  LineChart,
  XAxis,
  YAxis,
  Tooltip,
  Legend,
  ReferenceArea,
} from "recharts";
import "./App.css";

interface DataPoint {
  THERM_01: number;
  THERM_02: number;
  THERM_03: number;
  THERM_04: number;
  HTR_01: string;
  HTR_02: string;
  HTR_03: string;
  HTR_04: string;
  TIMESTAMP: string;
}

const App: React.FC = () => {
  const [displayData, setDisplayData] = useState<DataPoint[]>([]);
  const incomingData = useRef<DataPoint[]>([]);
  const ws = useRef<WebSocket | null>(null);

  const handleSocket = () => {
    console.log("Attempting to connect to WebSocket...");
    ws.current = new WebSocket("ws://localhost:8084");

    ws.current.onopen = () => {
      console.log("WebSocket connected");
    };

    ws.current.onmessage = (msg: MessageEvent) => {
      console.log("Message received:", msg.data);
      try {
        const parsedData = eval(msg.data); // Use eval to handle non-JSON data format
        console.log("Parsed data:", parsedData);
        const newData: DataPoint = {
          THERM_01: parsedData[0],
          THERM_02: parsedData[1],
          THERM_03: parsedData[2],
          THERM_04: parsedData[3],
          HTR_01: parsedData[4],
          HTR_02: parsedData[5],
          HTR_03: parsedData[6],
          HTR_04: parsedData[7],
          TIMESTAMP: new Date(parsedData[8]).toISOString(), // Ensure TIMESTAMP is a valid ISO string
        };
        incomingData.current.push(newData);
      } catch (error) {
        console.error("Error parsing message data:", error);
      }
    };

    ws.current.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    ws.current.onclose = () => {
      console.log("WebSocket disconnected, attempting to reconnect...");
      // Try to reconnect in 5 seconds
      setTimeout(handleSocket, 5000);
    };
  };

  useEffect(() => {
    handleSocket();
    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  useEffect(() => {
    const updateDisplayData = () => {
      const now = new Date().getTime();
      const oneSecondAgo = now - 1000;

      setDisplayData((prevDisplayData) => {
        const newDisplayData = [
          ...prevDisplayData.filter(
            (d) => new Date(d.TIMESTAMP).getTime() > oneSecondAgo
          ),
          ...incomingData.current.filter(
            (d) => new Date(d.TIMESTAMP).getTime() > oneSecondAgo
          ),
        ];
        console.log("Display data updated:", newDisplayData);
        incomingData.current = []; // Clear incoming data buffer
        return newDisplayData;
      });
    };
    const interval = setInterval(updateDisplayData, 1000); // Update display data every second
    return () => clearInterval(interval);
  }, []);

  const getReferenceAreas = (data: DataPoint[], heaterKey: keyof DataPoint) => {
    const referenceAreas: { x1: string; x2: string }[] = [];
    let start: string | null = null;
    data.forEach((entry) => {
      if (entry[heaterKey] === "On" && start === null) {
        start = entry.TIMESTAMP;
      } else if (entry[heaterKey] === "Off" && start !== null) {
        referenceAreas.push({ x1: start, x2: entry.TIMESTAMP });
        start = null;
      }
    });
    if (start !== null) {
      referenceAreas.push({ x1: start, x2: data[data.length - 1].TIMESTAMP });
    }
    return referenceAreas;
  };

  const renderCustomLegend = () => (
    <div className="custom-legend">
      <span style={{ color: "#8884d8" }}>Temperature</span>
      <span style={{ color: "rgba(255, 0, 0, 0.3)" }}> Heater On</span>
    </div>
  );

  const thermKeys: (keyof DataPoint)[] = [
    "THERM_01",
    "THERM_02",
    "THERM_03",
    "THERM_04",
  ];
  const heaterKeys: (keyof DataPoint)[] = [
    "HTR_01",
    "HTR_02",
    "HTR_03",
    "HTR_04",
  ];

  return (
    <>
      <h1>Temperature Data Charts</h1>
      <div className="chart-wrapper">
        <div className="chart-container">
          {thermKeys.map((thermKey, index) => (
            <div key={thermKey} className="chart-item">
              <h2>{thermKey}</h2>
              <LineChart width={400} height={300} data={displayData}>
                <Line type="monotone" dataKey={thermKey} stroke="#8884d8" />
                <CartesianGrid stroke="#ccc" />
                <XAxis
                  dataKey="TIMESTAMP"
                  tickFormatter={(tick) => new Date(tick).toLocaleTimeString()}
                />
                <YAxis />
                <Tooltip
                  labelFormatter={(label) =>
                    new Date(label).toLocaleTimeString()
                  }
                />
                <Legend content={renderCustomLegend} />
                {getReferenceAreas(displayData, heaterKeys[index]).map(
                  (area, i) => (
                    <ReferenceArea
                      key={i}
                      x1={area.x1}
                      x2={area.x2}
                      strokeOpacity={0.3}
                      fill="red"
                    />
                  )
                )}
              </LineChart>
            </div>
          ))}
        </div>
      </div>
    </>
  );
};

export default App;
