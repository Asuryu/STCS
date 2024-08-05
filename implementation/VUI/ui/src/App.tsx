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
import Footer from "./Footer";
import Toast from "./error_log"; // Ensure the path is correct
import { Slider } from "./components/ui/slider";
import { Label2 } from "@/components/ui/label";

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
  ENVIRONMENT: string;
  ERROR: string;
}

const App: React.FC = () => {
  const [displayData, setDisplayData] = useState<DataPoint[]>([]);
  const [LastData, setLastData] = useState<DataPoint | null>(null);
  const [errorLogs, setErrorLogs] = useState<string[]>([]);
  const [isSidePanelVisible, setIsSidePanelVisible] = useState<boolean>(false);
  const [currentError, setCurrentError] = useState<string | null>(null);
  const [refreshTime, setRefreshTime] = useState<number>(0.2);
  const [rangeTime, setRangeTime] = useState<number>(5);
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
          ERROR: parsedData[10],
          TIMESTAMP: new Date(parsedData[8]).toISOString(), // Ensure TIMESTAMP is a valid ISO string
          ENVIRONMENT: parsedData[9],
        };
        incomingData.current.push(newData);
        setLastData(newData);

        // Check for errors and update currentError and errorLogs
        if (newData.ERROR && newData.ERROR != "null") {
          setCurrentError(
            `Error at ${new Date(newData.TIMESTAMP).toLocaleTimeString()}: ${
              newData.ERROR
            }`
          );
          setErrorLogs((prevLogs) => [
            ...prevLogs,
            `Error at ${new Date(newData.TIMESTAMP).toLocaleTimeString()}: ${
              newData.ERROR
            }`,
          ]);
        } else {
          setCurrentError(null);
        }
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
      const timeWindow = now - rangeTime * 1000;

      setDisplayData((prevDisplayData) => {
        const newDisplayData = [
          ...prevDisplayData.filter(
            (d) => new Date(d.TIMESTAMP).getTime() > timeWindow
          ),
          ...incomingData.current.filter(
            (d) => new Date(d.TIMESTAMP).getTime() > timeWindow
          ),
        ];
        console.log("Display data updated:", newDisplayData);
        incomingData.current = []; // Clear incoming data buffer
        return newDisplayData;
      });
    };
    const interval = setInterval(updateDisplayData, refreshTime * 1000); // Update display data based on refreshTime
    return () => clearInterval(interval);
  }, [refreshTime, rangeTime]);

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

  const handleSlider1Change = (value) => {
    setRefreshTime(value);
  };
  const handleSlider2Change = (value) => {
    setRangeTime(value);
  };

  return (
    <>
      <h1 className="my-10 text-4xl font-bold">Temperature Data Charts</h1>
      <button 
        className="p-2 bg-red-600 text-white rounded"
        onClick={() => setIsSidePanelVisible(!isSidePanelVisible)}>
        Show Errors
        </button>
      <div className="mb-10 mt-1 p-4 w-full flex">
        <div className="mx-10 p-4 w-1/2">
          <div className="mb-10">
            <Label2 className="text-xl font-bold" htmlFor="Slider1">
              Adjust Data Frequency
            </Label2>
          </div>
          <Slider
            id="Slider1"
            defaultValue={[0.2]}
            min={0.2}
            max={1}
            step={0.2}
            onValueChange={handleSlider1Change}
          />
          <div className="">{refreshTime}s</div>
        </div>
        <div className="mx-10 p-4 w-1/2">
          <div className="mb-10">
            <Label2 className="text-xl font-bold" htmlFor="Slider2">
              Adjust Data Range
            </Label2>
          </div>
          <Slider
            id="Slider2"
            defaultValue={[5]}
            min={1}
            max={75}
            step={5}
            onValueChange={handleSlider2Change}
          />
          <div className="">{rangeTime}s</div>
        </div>
      </div>
      <div className="chart-wrapper mb-40">
        <div className="chart-container">
          {thermKeys.map((thermKey, index) => (
            <div key={thermKey} className="chart-item">
              <h2>{thermKey}</h2>
              <LineChart width={450} height={300} data={displayData}>
                <Line type="monotone" dataKey={thermKey} stroke="#8884d8" />
                <CartesianGrid stroke="#ccc" />
                <XAxis
                  dataKey="TIMESTAMP"
                  tickFormatter={(tick) => {
                    const now = new Date().getTime();
                    const tickTime = new Date(tick).getTime();
                    return `${((now - tickTime) / 1000).toFixed(1)}s ago`;
                  }}
                />
                <YAxis />
                <Tooltip
                  labelFormatter={(label) => {
                    const now = new Date().getTime();
                    const labelTime = new Date(label).getTime();
                    return `${((now - labelTime) / 1000).toFixed(1)}s ago`;
                  }}
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
      <Footer LastData={LastData} />

      {/* Conditional Toast Rendering */}
      {!isSidePanelVisible && currentError && (
        <Toast
          message={currentError}
          onClose={() => setCurrentError(null)}
          onClick={() => setIsSidePanelVisible(true)} // Show side panel on click
        />
      )}
      {/* Side Panel */}
      {isSidePanelVisible && (
        <div className="fixed top-0 right-0 h-full w-80 bg-gray-800 text-white p-4 shadow-lg z-40 overflow-y-auto">
          <h2 className="text-lg font-semibold mb-4">Error Log</h2>
          <button
            className="mb-4 bg-red-600 text-white px-4 py-2 rounded"
            onClick={() => setIsSidePanelVisible(false)}
          >
            Close
          </button>
          <ul className="space-y-2">
            {errorLogs
              .slice()
              .reverse()
              .map((log, index) => (
                <li key={index} className="border-b border-gray-600 pb-2">
                  {log}
                </li>
              ))}
          </ul>
        </div>
      )}
    </>
  );
};

export default App;
