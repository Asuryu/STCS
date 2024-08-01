import './App.css'
import { useEffect } from 'react'
import Footer from './Footer'
import Dashboard from './Dashboard'

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

  const status = "on"
  
  return (
    <>
      <h1>oi12</h1>
      <div className="flex flex-col min-h-screen">
      <div className="flex-grow">
        {/* Other components and content */}
      </div>
      <Footer status={status} />
    </div>
    </>
  )
}

export default App;
