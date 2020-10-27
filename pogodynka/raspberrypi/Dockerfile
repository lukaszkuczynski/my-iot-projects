FROM python
MAINTAINER kuczynskilukasz@gmail.com

ADD ./requirements.txt /requirements.txt
RUN pip install -r /requirements.txt

ENTRYPOINT ["python", "/real_device.py"]
ADD ./*.py /
