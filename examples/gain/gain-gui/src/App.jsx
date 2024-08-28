import {useState, useEffect} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    return (
        <>
            <div>
                <p className="label">Gain</p>
                <SliderComponent pid={window.params.kGain} name={"hello"} min={0} max={1}/>
            </div>
        </>
    )
}

export default App
