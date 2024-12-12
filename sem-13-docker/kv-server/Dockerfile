FROM python:3

WORKDIR /app

COPY ./requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY ./src ./src

ENTRYPOINT [ "python" ]
CMD [ "./src/run.py", "9090" ]
EXPOSE 9090
