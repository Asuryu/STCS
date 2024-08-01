import './App.css'
import { useEffect } from 'react'
import Footer from './Footer'

function App() {
  const handleSocket = () => {
    const ws = new WebSocket('ws://server:8084')
    ws.onopen = () => {
      console.log('connected')
    }
    ws.onmessage = (msg) => {
      console.log(msg.data)
    }
    ws.onclose = () => {
      console.log('disconnected')
    }
  }

  useEffect(() => {
    handleSocket();
  }, [])

  const items = [
    { name: 'Object 1', temperature: 25, status: 'on' },
    { name: 'Object 2', temperature: 22, status: 'off' },
    { name: 'Object 3', temperature: 28, status: 'on' },
    { name: 'Object 4', temperature: 20, status: 'off' },
  ];
  
  return (
    <>
      <h1>oi12</h1>
      <div className="flex flex-col min-h-screen">
      <div className="flex-grow">
        {/* Other components and content */}
      </div>
      <Footer items={items} />
    </div>
    </>
  )
}

export default App;
