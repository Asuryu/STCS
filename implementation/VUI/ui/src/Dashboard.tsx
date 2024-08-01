import React, { useState, useEffect } from "react";
import HeaterDisplay from "./HeaterDisplay";
import DeviceStatus from "./DeviceStatus";

const Dashboard: React.FC = () => {
  const [temperatures, setTemperatures] = useState<number[]>([0, 0, 0, 0]);
  const [deviceStates, setDeviceStates] = useState<boolean[]>([false, false, false, false]);

  useEffect(() => {
    const interval = setInterval(() => {
      setTemperatures([
        Math.random() * 100,
        Math.random() * 100,
        Math.random() * 100,
        Math.random() * 100,
      ]);
      setDeviceStates([
        Math.random() > 0.5,
        Math.random() > 0.5,
        Math.random() > 0.5,
        Math.random() > 0.5,
      ]);
    }, 5000);

    return () => clearInterval(interval);
  }, []);

  return (
    <div className="max-w-4xl mx-auto p-8 grid grid-cols-1 md:grid-cols-2 gap-8">
      <h1 className="text-4xl font-bold mb-8 col-span-1 md:col-span-2 text-center text-gray-800">
        Temperature Control Panel
      </h1>
      <div className="flex flex-col gap-6">
        {temperatures.map((temp, index) => (
          <HeaterDisplay key={index} temperature={temp} index={index} />
        ))}
      </div>
      <div className="flex flex-col gap-6">
        {deviceStates.map((state, index) => (
          <DeviceStatus key={index} isOn={state} index={index} />
        ))}
      </div>
    </div>
  );
};

export default Dashboard;
