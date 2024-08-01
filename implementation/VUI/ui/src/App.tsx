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
import { useEffect } from "react";

function App() {
  const handleSocket = () => {
    const ws = new WebSocket("ws://server:8084");
    ws.onopen = () => {
      console.log("connected");
    };
    ws.onmessage = (msg) => {
      console.log(msg.data);
    };
    ws.onclose = () => {
      console.log("disconnected");
    };
  };

  useEffect(() => {
    handleSocket();
  }, []);

  const data = [
    {
      THERM_01: -5,
      THERM_02: 5,
      THERM_03: 1,
      THERM_04: -5,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:27:59.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 6,
      THERM_03: 2,
      THERM_04: -4,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:28:09.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 7,
      THERM_03: 3,
      THERM_04: -3,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:28:19.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 8,
      THERM_03: 4,
      THERM_04: -2,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:28:29.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 5,
      THERM_03: 5,
      THERM_04: -1,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:28:39.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 6,
      THERM_03: 1,
      THERM_04: 0,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:28:49.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 7,
      THERM_03: 2,
      THERM_04: -5,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:28:59.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 8,
      THERM_03: 3,
      THERM_04: -4,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:29:09.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 5,
      THERM_03: 4,
      THERM_04: -3,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:29:19.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 6,
      THERM_03: 5,
      THERM_04: -2,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:29:29.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 7,
      THERM_03: 1,
      THERM_04: -1,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:29:39.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 6,
      THERM_03: 2,
      THERM_04: 0,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:29:49.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 5,
      THERM_03: 3,
      THERM_04: -3,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:29:59.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 8,
      THERM_03: 4,
      THERM_04: -4,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:30:09.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 7,
      THERM_03: 5,
      THERM_04: -1,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:30:19.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 6,
      THERM_03: 1,
      THERM_04: 0,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:30:29.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 5,
      THERM_03: 2,
      THERM_04: -3,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:30:39.577897",
    },
    {
      THERM_01: -3,
      THERM_02: 8,
      THERM_03: 3,
      THERM_04: -4,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:30:49.577897",
    },
    {
      THERM_01: -5,
      THERM_02: 7,
      THERM_03: 4,
      THERM_04: -5,
      HTR_01: "On",
      HTR_02: "Off",
      HTR_03: "On",
      HTR_04: "Off",
      TIMESTAMP: "2024-08-01T08:30:59.577897",
    },
    {
      THERM_01: -4,
      THERM_02: 6,
      THERM_03: 5,
      THERM_04: -2,
      HTR_01: "Off",
      HTR_02: "On",
      HTR_03: "Off",
      HTR_04: "On",
      TIMESTAMP: "2024-08-01T08:31:09.577897",
    },
  ];

  const getReferenceAreas = (data, heaterKey) => {
    const referenceAreas = [];
    let start = null;
    data.forEach((entry, index) => {
      if (entry[heaterKey] === "On" && start === null) {
        start = entry.TIMESTAMP;
      } else if (entry[heaterKey] === "Off" && start !== null) {
        referenceAreas.push({ x1: start, x2: entry.TIMESTAMP });
        start = null;
      }
    });
    // In case the heater was on till the end
    if (start !== null) {
      referenceAreas.push({ x1: start, x2: data[data.length - 1].TIMESTAMP });
    }
    return referenceAreas;
  };

  const renderCustomLegend = () => {
    return (
      <div className="custom-legend">
        <span style={{ color: "#8884d8" }}>Temperature</span>
        <span style={{ color: "rgba(255, 0, 0, 0.3)" }}> Heater On</span>
      </div>
    );
  };

  const thermKeys = ["THERM_01", "THERM_02", "THERM_03", "THERM_04"];
  const heaterKeys = ["HTR_01", "HTR_02", "HTR_03", "HTR_04"];

  return (
    <>
      <h1>Temperature Data Charts</h1>
      <div className="chart-wrapper">
        <div className="chart-container">
          {thermKeys.map((thermKey, index) => (
            <div key={thermKey} className="chart-item">
              <h2>{thermKey}</h2>
              <LineChart width={400} height={300} data={data}>
                <Line type="monotone" dataKey={thermKey} stroke="#8884d8" />
                <CartesianGrid stroke="#ccc" />
                <XAxis dataKey="TIMESTAMP" />
                <YAxis />
                <Tooltip />
                <Legend content={renderCustomLegend} />
                {getReferenceAreas(data, heaterKeys[index]).map((area, i) => (
                  <ReferenceArea
                    key={i}
                    x1={area.x1}
                    x2={area.x2}
                    strokeOpacity={0.3}
                    fill="red"
                  />
                ))}
              </LineChart>
            </div>
          ))}
        </div>
      </div>
    </>
  );
}

export default App;
