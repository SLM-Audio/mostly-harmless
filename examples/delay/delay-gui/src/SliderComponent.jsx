import {useEffect, useState} from "react";

function SliderComponent({pid, name, min, max}) {
    const [val, setVal] = useState(0.0);
    useEffect(() => {
        addEventListener("param", (ev) => {
            const eventPid = ev.detail.paramId;
            if (eventPid !== pid) return;
            setVal(parseFloat(ev.detail.value));
        });
    }, []);

    const sliderDragStarted = (v) => {
        beginParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        const args = {
            paramId: 0,
            value: v.target.value
        };
        setParamValue(args).then((res) => console.log(res));
    };

    const sliderDragEnded = (v) => {
        endParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    return (
        <>
            <input className="slider-el"
                   type={"range"}
                   value={val}
                   onDragStart={sliderDragStarted}
                   onChange={sliderChangedCallback}
                   onDragEnd={sliderDragEnded}
                   min={min} max={max} step={0.00001}></input>

        </>
    )
}

export default SliderComponent;